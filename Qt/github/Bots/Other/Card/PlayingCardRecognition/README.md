#Know Your Cards

Recognize playing cards using Windows Phone as a client and Microsoft Asp.net WebApi as a server . The project is inspired from Nazmi Altun and his article on codeproject that is currently unavailable. 

### Projects
There are 2 projects here, the one would be ImageUploader and ImageUploadServer. 

- ImageUploader is a simple Windows Phone app that takes a picture from gallery and uploads it. If you want a CameraCaptureTask, feel free to upload it. 
- ImageUploadServer takes the image, uses Aforge.net and with a little basic template matching he processes the image and gets you the playing cards name.

(I will write a full description on how it works soon here)

#Ways to use it:
####Binding your IP in IIS Express
- Find IIS Express configuration file, applicationhost.config, in the folder %USERPROFILE%\Documents\IISExpress\config\  
- Duplicate the <binding> line, and replace “localhost” with your IP address (make sure to verify the port number). Save the file.
- At this point, you should be able to run the web service and on the browser, go to the /fileupload using your IP address and port instead of localhost. However, that works only from the browser.

Note: you might not be able to reach out the endpoint with your IP address. If that is the case, completing the following steps, and re-running the web service should fix it.

####Allow Incoming Connections

If you try to reach the endpoint on your browser using the IP address, you will most likely get a Bad Request or Service Unavailable response, since up to this point the firewall is not allowing incoming connections.

- Start a command prompt with administrative privileges.
- Run the following command making sure to include your IP address and the correct app port: ``` netsh http add urlacl url=http://youripaddress:yourappport/ user=everyone. ``` This just tells http.sys that it’s ok to talk to this url.
- Add a rule in the Windows Firewall, allowing incoming connections to your app port for computers on your local subnet. To do so, run the following command including the correct port.
```
netsh advfirewall firewall add rule name="IISExpressWeb" dir=in protocol=tcp localport=yourappport profile=private remoteip=localsubnet action=allow
```

If you open the Windows Firewall manager, you should be able to see the new rule added. You can change or delete the rule from here easily.

###Change the ip on the app
Go over MainPage.xaml.cs and UploadFile() method and change the following line to your server api

```
var fileUploadUrl = @"http://10.211.10.190:60662/api/file";
```

###Bugs
- Cant detect cards if they are put on one over another
- Usually works best if a dark colored background is provided

###Updates that are coming
- Multiple card recognition
- Card boundary visbility

Let me know how you guys like it and have fun!







