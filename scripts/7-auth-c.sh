curl -X POST -H"authorization:Basic aHRtbDU6cGFzc3dvcmQ=" -F"password=bot" -F"client_secret=password" -F"client=html5" -F"username=pwebb" -F"grant_type=password" -F"scope=openid"  http://localhost:${OAUTH_HOST_PORT_C}/uaa/oauth/token | jq