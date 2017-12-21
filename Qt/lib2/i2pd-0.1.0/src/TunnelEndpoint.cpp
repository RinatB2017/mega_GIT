#include "I2PEndian.h"
#include <string.h>
#include "Log.h"
#include "NetDb.h"
#include "I2NPProtocol.h"
#include "Transports.h"
#include "RouterContext.h"
#include "TunnelEndpoint.h"

namespace i2p
{
namespace tunnel
{
	TunnelEndpoint::~TunnelEndpoint ()
	{
		for (auto it: m_IncompleteMessages)
			i2p::DeleteI2NPMessage (it.second.data);
		for (auto it: m_OutOfSequenceFragments)
			i2p::DeleteI2NPMessage (it.second.data);
	}	
	
	void TunnelEndpoint::HandleDecryptedTunnelDataMsg (I2NPMessage * msg)
	{
		m_NumReceivedBytes += TUNNEL_DATA_MSG_SIZE;
		
		uint8_t * decrypted = msg->GetPayload () + 20; // 4 + 16
		uint8_t * zero = (uint8_t *)memchr (decrypted + 4, 0, TUNNEL_DATA_ENCRYPTED_SIZE - 4); // witout 4-byte checksum
		if (zero)
		{	
			LogPrint ("TunnelMessage: zero found at ", (int)(zero-decrypted));
			uint8_t * fragment = zero + 1;
			// verify checksum
			memcpy (msg->GetPayload () + TUNNEL_DATA_MSG_SIZE, msg->GetPayload () + 4, 16); // copy iv to the end
			uint8_t hash[32];
			CryptoPP::SHA256().CalculateDigest (hash, fragment, TUNNEL_DATA_MSG_SIZE -(fragment - msg->GetPayload ()) + 16); // payload + iv
			if (memcmp (hash, decrypted, 4))
			{
				LogPrint ("TunnelMessage: checksum verification failed");
				i2p::DeleteI2NPMessage (msg);
				return;
			}	
			// process fragments
			while (fragment < decrypted + TUNNEL_DATA_ENCRYPTED_SIZE)
			{
				uint8_t flag = fragment[0];
				fragment++;
				
				bool isFollowOnFragment = flag & 0x80, isLastFragment = true;		
				uint32_t msgID = 0;
				int fragmentNum = 0;
				TunnelMessageBlockEx m;
				if (!isFollowOnFragment)
				{	
					// first fragment
					
					m.deliveryType = (TunnelDeliveryType)((flag >> 5) & 0x03);
					switch (m.deliveryType)
					{
						case eDeliveryTypeLocal: // 0
							LogPrint ("Delivery type local");
						break;
					  	case eDeliveryTypeTunnel: // 1
							LogPrint ("Delivery type tunnel");	
							m.tunnelID = be32toh (*(uint32_t *)fragment);
							fragment += 4; // tunnelID
							m.hash = i2p::data::IdentHash (fragment);
							fragment += 32; // hash
						break;
						case eDeliveryTypeRouter: // 2
							LogPrint ("Delivery type router");	
							m.hash = i2p::data::IdentHash (fragment);	
							fragment += 32; // to hash
						break;
						default:
							;
					}	

					bool isFragmented = flag & 0x08;
					if (isFragmented)
					{
						// Message ID
						msgID = be32toh (*(uint32_t *)fragment); 	
						fragment += 4;
						LogPrint ("Fragmented message ", msgID);
						isLastFragment = false;
					}	
				}
				else
				{
					// follow on
					msgID = be32toh (*(uint32_t *)fragment); // MessageID			
					fragment += 4; 
					fragmentNum = (flag >> 1) & 0x3F; // 6 bits
					isLastFragment = flag & 0x01;
					LogPrint ("Follow on fragment ", fragmentNum, " of message ", msgID, isLastFragment ? " last" : " non-last");
				}	
				
				uint16_t size = be16toh (*(uint16_t *)fragment);
				fragment += 2;
				LogPrint ("Fragment size=", (int)size);

				msg->offset = fragment - msg->buf;
				msg->len = msg->offset + size;
				if (fragment + size < decrypted + TUNNEL_DATA_ENCRYPTED_SIZE)
				{
					// this is not last message. we have to copy it
					m.data = NewI2NPMessage ();
					m.data->offset += sizeof (TunnelGatewayHeader); // reserve room for TunnelGateway header
					m.data->len += sizeof (TunnelGatewayHeader);
					*(m.data) = *msg;
				}
				else
					m.data = msg;
				
				if (!isFollowOnFragment && isLastFragment)
					HandleNextMessage (m);
				else
				{
					if (msgID) // msgID is presented, assume message is fragmented
					{
						if (!isFollowOnFragment) // create new incomlete message
						{
							m.nextFragmentNum = 1;
							auto& msg = m_IncompleteMessages[msgID];
							msg = m;
							HandleOutOfSequenceFragment (msgID, msg);
						}
						else
						{
							m.nextFragmentNum = fragmentNum;
							HandleFollowOnFragment (msgID, isLastFragment, m);
						}	
					}
					else
						LogPrint ("Message is fragmented, but msgID is not presented");
				}	
					
				fragment += size;
			}	
		}	
		else
		{	
			LogPrint ("TunnelMessage: zero not found");
			i2p::DeleteI2NPMessage (msg);	
		}	
	}	

	void TunnelEndpoint::HandleFollowOnFragment (uint32_t msgID, bool isLastFragment, const TunnelMessageBlockEx& m)
	{
		auto fragment = m.data->GetBuffer ();
		auto size = m.data->GetLength ();
		auto it = m_IncompleteMessages.find (msgID);
		if (it != m_IncompleteMessages.end())
		{
			auto& msg = it->second;
			if (m.nextFragmentNum == msg.nextFragmentNum)
			{
				if (msg.data->len + size < I2NP_MAX_MESSAGE_SIZE) // check if messega is not too long
				{	
					memcpy (msg.data->buf + msg.data->len, fragment, size); // concatenate fragment
					msg.data->len += size;
					if (isLastFragment)
					{
						// message complete
						HandleNextMessage (msg);	
						m_IncompleteMessages.erase (it); 
					}	
					else
					{	
						msg.nextFragmentNum++;
						HandleOutOfSequenceFragment (msgID, msg);
					}	
				}
				else
				{
					LogPrint ("Fragment ", m.nextFragmentNum, " of message ", msgID, "exceeds max I2NP message size. Message dropped");
					i2p::DeleteI2NPMessage (msg.data);
					m_IncompleteMessages.erase (it);
				}
				i2p::DeleteI2NPMessage (m.data);
			}
			else
			{	
				LogPrint ("Unexpected fragment ", (int)m.nextFragmentNum, " instead ", (int)msg.nextFragmentNum, " of message ", msgID, ". Saved");
				AddOutOfSequenceFragment (msgID, m.nextFragmentNum, isLastFragment, m.data);
			}
		}
		else
		{	
			LogPrint ("First fragment of message ", msgID, " not found. Saved");
			AddOutOfSequenceFragment (msgID, m.nextFragmentNum, isLastFragment, m.data);
		}	
	}	

	void TunnelEndpoint::AddOutOfSequenceFragment (uint32_t msgID, uint8_t fragmentNum, bool isLastFragment, I2NPMessage * data)
	{
		auto it = m_OutOfSequenceFragments.find (msgID);
		if (it == m_OutOfSequenceFragments.end ())
			m_OutOfSequenceFragments.insert (std::pair<uint32_t, Fragment> (msgID, {fragmentNum, isLastFragment, data}));	
		else
			i2p::DeleteI2NPMessage (data);
	}	

	void TunnelEndpoint::HandleOutOfSequenceFragment (uint32_t msgID, TunnelMessageBlockEx& msg)
	{
		auto it = m_OutOfSequenceFragments.find (msgID);
		if (it != m_OutOfSequenceFragments.end ())
		{
			if (it->second.fragmentNum == msg.nextFragmentNum)
			{
				LogPrint ("Out-of-sequence fragment ", (int)it->second.fragmentNum, " of message ", msgID, " found");
				auto size = it->second.data->GetLength ();
				memcpy (msg.data->buf + msg.data->len, it->second.data->GetBuffer (), size); // concatenate out-of-sync fragment
				msg.data->len += size;
				if (it->second.isLastFragment)
				{
					// message complete
					HandleNextMessage (msg);	
					m_IncompleteMessages.erase (msgID); 
				}	
				else
					msg.nextFragmentNum++;
				i2p::DeleteI2NPMessage (it->second.data);
				m_OutOfSequenceFragments.erase (it);
			}	
		}	
	}	
	
	void TunnelEndpoint::HandleNextMessage (const TunnelMessageBlock& msg)
	{
		LogPrint ("TunnelMessage: handle fragment of ", msg.data->GetLength ()," bytes. Msg type ", (int)msg.data->GetHeader()->typeID);
		switch (msg.deliveryType)
		{
			case eDeliveryTypeLocal:
				i2p::HandleI2NPMessage (msg.data);
			break;
			case eDeliveryTypeTunnel:
				i2p::transports.SendMessage (msg.hash, i2p::CreateTunnelGatewayMsg (msg.tunnelID, msg.data));
			break;
			case eDeliveryTypeRouter:
				if (msg.hash == i2p::context.GetRouterInfo ().GetIdentHash ()) // check if message is sent to us
					i2p::HandleI2NPMessage (msg.data);
				else
				{	
					// to somebody else
					if (!m_IsInbound) // outbound transit tunnel
					{
						if (msg.data->GetHeader()->typeID == eI2NPDatabaseStore ||
						    msg.data->GetHeader()->typeID == eI2NPDatabaseSearchReply )
						{
							// catch RI or reply with new list of routers
							auto ds = NewI2NPMessage ();
							*ds = *(msg.data);
							i2p::data::netdb.PostI2NPMsg (ds);
						}
						i2p::transports.SendMessage (msg.hash, msg.data);
					}
					else // we shouldn't send this message. possible leakage 
					{
						LogPrint ("Message to another router arrived from an inbound tunnel. Dropped");
						i2p::DeleteI2NPMessage (msg.data);
					}
				}
			break;
			default:
				LogPrint ("TunnelMessage: Unknown delivery type ", (int)msg.deliveryType);
		};	
	}	
}		
}
