 
 
 # FleXd Logger/Trace/Debug Application
 
 This part of FleXd project contains application for storing logs into central location (server) and logger implementation interface for many applications (client).
 
 ## About
 This applications are implement as client-server model. Client and server communicate through ICL (https://github.com/FleX-d/ICL).
 
 ## FleXd Logger Server
 
 Purpose of this application is receiving logs from system applications and all aplications that run on the FleXd distribution that implement logger macros from client part.
 
 #### Server can log into:
    -syslog (syslog-ng)
    -dlt -> https://github.com/GENIVI/dlt-viewer
    ...
 
 #### More informations

 Link: https://github.com/FleX-d/LTD/tree/dev/apps/fleXdLoggerServer
 
 ## FleXd Logger Client
 
 It is library that implement logger client. 
 
 #### More informations
 Link: https://github.com/FleX-d/LTD/tree/dev/libs/fleXdLoggerClient
 
