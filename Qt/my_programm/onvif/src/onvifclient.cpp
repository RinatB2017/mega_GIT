// onvifclient.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include "include/onvifclient.hpp"


int main(int argc, char* argv[])
{
    int ret;
    /* 192.168.1.1 is the NVT, 192.168.1.234 is the NVC */
    string url = "http://192.168.1.88/onvif/device_service";
    /* Below is where to receive the event */
    string eventNotify = "http://192.168.1.88:9090/subscription-2";
    string user = "admin";
    string pass =  "admin";
    OnvifClientDevice onvifDevice(url, user, pass);
    _tds__GetCapabilitiesResponse capabilitiesResponse;

    onvifDevice.GetCapabilities();

    OnvifClientMedia media(onvifDevice);
    _trt__GetProfilesResponse profiles;
    media.GetProfiles(profiles);

    //GetPresetTours
    PTZBindingProxy ptzBindingProxy(SOAP_C_UTFSTRING);
    _tptz__GetPresetTours req;
    req.ProfileToken = profiles.Profiles[0]->token;
    _tptz__GetPresetToursResponse res;
    ptzBindingProxy.soap_endpoint =  url.c_str();
    soap_wsse_add_Security(&ptzBindingProxy);
    soap_wsse_add_UsernameTokenDigest(&ptzBindingProxy, "Id", user.c_str() , pass.c_str());
    int reSoap = ptzBindingProxy.GetPresetTours(&req, &res);


    _tptz__GetPresets GetPresets;
    GetPresets.ProfileToken = profiles.Profiles[0]->token;
    _tptz__GetPresetsResponse GetPresetsResponse;
    ptzBindingProxy.soap_endpoint =  url.c_str();
    soap_wsse_add_Security(&ptzBindingProxy);
    soap_wsse_add_UsernameTokenDigest(&ptzBindingProxy, "Id", user.c_str() , pass.c_str());
    reSoap = ptzBindingProxy.GetPresets(&GetPresets, &GetPresetsResponse);



    _tptz__ModifyPresetTour ModifyPresetTour;
    ModifyPresetTour.PresetTour = res.PresetTour[0];
    ModifyPresetTour.ProfileToken = profiles.Profiles[0]->token;
    while (ModifyPresetTour.PresetTour->TourSpot.size()>0)
    {
        ModifyPresetTour.PresetTour->TourSpot.pop_back();
    }
    for (int i=0; i<GetPresetsResponse.Preset.size(); i++)
    {
        tt__PTZPresetTourSpot *PTZPresetTourSpot  = new tt__PTZPresetTourSpot;      //( tt__PTZPresetTourSpot*)calloc(1, sizeof(tt__PTZPresetTourSpot));
        PTZPresetTourSpot->PresetDetail = new class tt__PTZPresetTourPresetDetail;  // ( tt__PTZPresetTourPresetDetail *)calloc(1,sizeof(tt__PTZPresetTourPresetDetail));
        LONG64 to = 5000;
        PTZPresetTourSpot->StayTime = &to;
        PTZPresetTourSpot->PresetDetail->__union_PTZPresetTourPresetDetail = 1;
        PTZPresetTourSpot->PresetDetail->union_PTZPresetTourPresetDetail.PresetToken = GetPresetsResponse.Preset[i]->token;
        ModifyPresetTour.PresetTour->TourSpot.push_back(PTZPresetTourSpot);
    }
    _tptz__ModifyPresetTourResponse ModifyPresetTourResponse;
    soap_wsse_add_Security(&ptzBindingProxy);
    soap_wsse_add_UsernameTokenDigest(&ptzBindingProxy, "Id", user.c_str() , pass.c_str());
    reSoap = ptzBindingProxy.ModifyPresetTour(&ModifyPresetTour, &ModifyPresetTourResponse);
    if(reSoap !=0)
    {
        printf("soap error: %d, %s, %s\n", ModifyPresetTourResponse.soap->error, *soap_faultcode(ModifyPresetTourResponse.soap), *soap_faultstring(ModifyPresetTourResponse.soap));
    }

    _tptz__OperatePresetTour OperatePresetTour;
    OperatePresetTour.Operation = tt__PTZPresetTourOperation__Start;
    OperatePresetTour.PresetTourToken = res.PresetTour[0]->token[0];
    OperatePresetTour.ProfileToken =  profiles.Profiles[0]->token;
    _tptz__OperatePresetTourResponse OperatePresetTourResponse;
    soap_wsse_add_Security(&ptzBindingProxy);
    soap_wsse_add_UsernameTokenDigest(&ptzBindingProxy, "Id", user.c_str() , pass.c_str());
    reSoap = ptzBindingProxy.OperatePresetTour(&OperatePresetTour, &OperatePresetTourResponse);
    if(reSoap !=0)
    {
        printf("soap error: %d, %s, %s\n", OperatePresetTourResponse.soap->error, *soap_faultcode(OperatePresetTourResponse.soap), *soap_faultstring(OperatePresetTourResponse.soap));
    }

#if 0
    /* Recording */
    OnvifClientRecording onvifRecording(onvifDevice);
    _trc__GetRecordingsResponse recordings;
    onvifRecording.GetRecordings(recordings);
    for (vector<tt__GetRecordingsResponseItem * >::iterator iter =
         recordings.RecordingItem.begin();
         iter < recordings.RecordingItem.end(); ++iter)
    {
        tt__GetRecordingsResponseItem * pRecordings = *iter;
        printf("pRecordings %s\n", pRecordings->RecordingToken.c_str());
        tt__RecordingConfiguration *pRecordingConf = pRecordings->Configuration;
        if (pRecordingConf->Source)
        {
            printf("Recording SourceId %s\n", pRecordingConf->Source->SourceId.c_str());
            printf("Recording Name %s\n", pRecordingConf->Source->Name.c_str());
            printf("Recording Location %s\n", pRecordingConf->Source->Location.c_str());
            printf("Recording Description %s\n", pRecordingConf->Source->Description.c_str());
            printf("Recording Address %s\n", pRecordingConf->Source->Address.c_str());
        }

        string replayUrl;
        OnvifClientReplay onvifRelay(onvifDevice);
        onvifRelay.GetReplayUri(pRecordings->RecordingToken, replayUrl);
        printf("Relay Url %s\n", replayUrl.c_str());
    }
#endif

#if 0
    /* Onvif Receiver */
    OnvifClientReceiver onvifReceiver(onvifDevice);
    _trv__GetReceiversResponse receivers;
    onvifReceiver.GetReceivers(receivers);
    for (vector<tt__Receiver * >::iterator iter =
         receivers.Receivers.begin();
         iter < receivers.Receivers.end(); ++iter)
    {
        tt__Receiver * pReceivers = *iter;
        printf("pReceivers %s\n", pReceivers->Token.c_str());
    }

    OnvifClientEvent onvifEvent(onvifDevice);

    onvifEvent.Subscribe(eventNotify);

    OnvifClientEventNotify notify(soap_new());

    if ((ret = soap_bind(&notify, NULL, 9090, 100) == SOAP_INVALID_SOCKET ) )
    {
        printf("OnvifClientEventNotify::soap_bind Binding on %d port failed", 9090);
        return 0;
    }
    int timeStart = time(NULL);
    int currentTime = 0;

    //Loop to recevie the event
    while(1)
    {
        currentTime = time(NULL);
        if (currentTime - timeStart > 30)
        {
            onvifEvent.Renew();
        }
        //printf("soap_accept accepting\n");
        if( (ret = soap_accept(&notify)) == SOAP_INVALID_SOCKET)
        {
            //printf("soap_accept accepting timeout\n");
            continue;
        }

        if ( (soap_begin_serve(&notify)) != SOAP_OK)
        {
            printf("soap_begin_serve serve %d failed", ret);
            continue;
        }

        ret = notify.dispatch();
        continue;
    }
#endif

    return 0;
}
