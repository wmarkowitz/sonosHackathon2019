import http.client

conn = http.client.HTTPConnection("api,ws,sonos,com")

payload = ""

headers = {
    'Content-Type': "application/x-www-form-urlencoded",
    'X-Sonos-Api-Key': "536e8328-d836-476b-8035-fd206794c9b7",
    'Host': "api.ws.sonos.com",
    'Authorization': "Bearer d5c9b056-ce40-454f-8423-35cfbb501742",
    'Cache-Control': "no-cache",
    'Postman-Token': "72111493-883b-45ca-8f0c-61a941db35a2"
    }

conn.request("POST", "control,api,v1,players,RINCON_7828CAF4D45E01400,playerVolume", payload, headers, body={"volume": 5})

res = conn.getresponse()
data = res.read()

print(data.decode("utf-8"))