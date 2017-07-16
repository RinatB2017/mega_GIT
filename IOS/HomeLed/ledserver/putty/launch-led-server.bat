putty pi@1.1.1.46 -m stop-java-process.txt
pscp c:/Users/jasf/IdeaProjects/ledserver/out/artifacts/ledserver/ledserver.jar pi@1.1.1.46:/home/pi
putty pi@1.1.1.46 -m launch_jar.txt
