#ifndef GARLIC_H__
#define GARLIC_H__

#include <inttypes.h>
#include <map>
#include <list>
#include <string>
#include <thread>
#include <mutex>
#include <memory>
#include <cryptopp/osrng.h>
#include "aes.h"
#include "I2NPProtocol.h"
#include "LeaseSet.h"
#include "Queue.h"
#include "Identity.h"

namespace i2p
{	
namespace garlic
{
	
	enum GarlicDeliveryType 
	{ 
		eGarlicDeliveryTypeLocal = 0, 
		eGarlicDeliveryTypeDestination = 1,
		eGarlicDeliveryTypeRouter = 2,	
		eGarlicDeliveryTypeTunnel = 3
	};	

#pragma pack(1)
	struct ElGamalBlock
	{
		uint8_t sessionKey[32];
		uint8_t preIV[32];
		uint8_t padding[158];
	};		
#pragma pack()	

	const int INCOMING_TAGS_EXPIRATION_TIMEOUT = 900; // 15 minutes			
	const int OUTGOING_TAGS_EXPIRATION_TIMEOUT = 720; // 12 minutes
	
	struct SessionTag: public i2p::data::Tag<32> 
	{
		SessionTag (const uint8_t * buf, uint32_t ts = 0): Tag<32>(buf), creationTime (ts) {};
		SessionTag () = default;
		SessionTag (const SessionTag& ) = default;
		SessionTag& operator= (const SessionTag& ) = default;
#ifndef _WIN32
		SessionTag (SessionTag&& ) = default; 
		SessionTag& operator= (SessionTag&& ) = default;	
#endif
		uint32_t creationTime; // seconds since epoch	
	};
		
	class GarlicDestination;
	class GarlicRoutingSession
	{
			struct UnconfirmedTags
			{
				UnconfirmedTags (int n): numTags (n), tagsCreationTime (0) { sessionTags = new SessionTag[numTags]; };
				~UnconfirmedTags () { delete[] sessionTags; };
				int numTags;
				SessionTag * sessionTags;
				uint32_t tagsCreationTime;
			};

		public:

			GarlicRoutingSession (GarlicDestination * owner, const i2p::data::RoutingDestination * destination, int numTags);
			GarlicRoutingSession (const uint8_t * sessionKey, const SessionTag& sessionTag); // one time encryption
			~GarlicRoutingSession ();
			I2NPMessage * WrapSingleMessage (I2NPMessage * msg);
			void TagsConfirmed (uint32_t msgID);

			void SetLeaseSetUpdated () { m_LeaseSetUpdated = true; };
			
		private:

			size_t CreateAESBlock (uint8_t * buf, const I2NPMessage * msg);
			size_t CreateGarlicPayload (uint8_t * payload, const I2NPMessage * msg, UnconfirmedTags * newTags);
			size_t CreateGarlicClove (uint8_t * buf, const I2NPMessage * msg, bool isDestination);
			size_t CreateDeliveryStatusClove (uint8_t * buf, uint32_t msgID);
			
			UnconfirmedTags * GenerateSessionTags ();

		private:

			GarlicDestination * m_Owner;
			const i2p::data::RoutingDestination * m_Destination;
			uint8_t m_SessionKey[32];
			std::list<SessionTag> m_SessionTags;
			int m_NumTags;
			std::map<uint32_t, UnconfirmedTags *> m_UnconfirmedTagsMsgs;	
			bool m_LeaseSetUpdated;

			i2p::crypto::CBCEncryption m_Encryption;
			CryptoPP::AutoSeededRandomPool m_Rnd;
	};	

	class GarlicDestination: public i2p::data::LocalDestination
	{
		public:

			GarlicDestination () {};
			~GarlicDestination ();

			GarlicRoutingSession * GetRoutingSession (const i2p::data::RoutingDestination& destination, int numTags);	
			I2NPMessage * WrapMessage (const i2p::data::RoutingDestination& destination, 
			    I2NPMessage * msg, bool attachLeaseSet = false);

			void AddSessionKey (const uint8_t * key, const uint8_t * tag); // one tag
			void DeliveryStatusSent (GarlicRoutingSession * session, uint32_t msgID);
			
			virtual void ProcessGarlicMessage (I2NPMessage * msg);
			virtual void ProcessDeliveryStatusMessage (I2NPMessage * msg);			
			virtual void SetLeaseSetUpdated ();
			
			virtual const i2p::data::LeaseSet * GetLeaseSet () = 0; // TODO
			virtual void HandleI2NPMessage (const uint8_t * buf, size_t len, i2p::tunnel::InboundTunnel * from) = 0;
			
		protected:

			void HandleGarlicMessage (I2NPMessage * msg);
			void HandleDeliveryStatusMessage (I2NPMessage * msg);			
	
		private:

			void HandleAESBlock (uint8_t * buf, size_t len, std::shared_ptr<i2p::crypto::CBCDecryption> decryption, 
				i2p::tunnel::InboundTunnel * from);
			void HandleGarlicPayload (uint8_t * buf, size_t len, i2p::tunnel::InboundTunnel * from);

		private:
			
			// outgoing sessions
			std::mutex m_SessionsMutex;
			std::map<i2p::data::IdentHash, GarlicRoutingSession *> m_Sessions;
			// incoming
			std::map<SessionTag, std::shared_ptr<i2p::crypto::CBCDecryption>> m_Tags;	
			// DeliveryStatus
			std::map<uint32_t, GarlicRoutingSession *> m_CreatedSessions; // msgID -> session
	};	
}	
}

#endif
