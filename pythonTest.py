import requests
import json
import asyncio

async def lowerVolume():
    url = "https://api.ws.sonos.com/control/api/v1/players/RINCON_7828CAF4D45E01400/playerVolume"

    payload = {"volume": 5}
    body = json.dumps(payload)
    headers = {
        'Content-Type': "application/x-www-form-urlencoded",
        'X-Sonos-Api-Key': "536e8328-d836-476b-8035-fd206794c9b7",
        'Host': "api.ws.sonos.com",
        'Authorization': "Bearer d5c9b056-ce40-454f-8423-35cfbb501742",
        'Cache-Control': "no-cache",
        'Postman-Token': "e19dae15-21d0-499f-94e5-559953e6bb31"
        }

    response = requests.request("POST", url, data=body, headers=headers)

    print(response.text)
    return(response.text)

async def raiseVolume():
    url = "https://api.ws.sonos.com/control/api/v1/players/RINCON_7828CAE736E601400/playerVolume"

    payload = {"volume": 70}
    body = json.dumps(payload)
    headers = {
        'Content-Type': "application/x-www-form-urlencoded",
        'X-Sonos-Api-Key': "536e8328-d836-476b-8035-fd206794c9b7",
        'Host': "api.ws.sonos.com",
        'Authorization': "Bearer d5c9b056-ce40-454f-8423-35cfbb501742",
        'Cache-Control': "no-cache",
        'Postman-Token': "e19dae15-21d0-499f-94e5-559953e6bb31"
        }
    response = requests.request("POST", url, data=body, headers=headers)
    print(response.text)
    return(response.text)

async def raiseRelativeVolume(volDelta):
    url = "https://api.ws.sonos.com/control/api/v1/players/RINCON_7828CAE736E601400/playerVolume/relative"

    payload = {"volumeDelta": volDelta}
    body = json.dumps(payload)
    headers = {
        'Content-Type': "application/x-www-form-urlencoded",
        'X-Sonos-Api-Key': "536e8328-d836-476b-8035-fd206794c9b7",
        'Host': "api.ws.sonos.com",
        'Authorization': "Bearer d5c9b056-ce40-454f-8423-35cfbb501742",
        'Cache-Control': "no-cache",
        'Postman-Token': "e19dae15-21d0-499f-94e5-559953e6bb31"
        }
    response = requests.request("POST", url, data=body, headers=headers)
    print(response.text)
    return(response.text)

async def main():
    await asyncio.gather(
        lowerVolume(),
        raiseVolume()
    )
    
asyncio.run(main())
