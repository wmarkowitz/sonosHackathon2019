import requests
import json
import asyncio
import numpy as np
bedroom = "RINCON_7828CAE736E601400"
bathroom = "RINCON_7828CAF4D45E01400"
# bathroom
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

# bedroom
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
    return(response.text)

async def raiseRelativeVolume(volDelta, id):
    url = f"https://api.ws.sonos.com/control/api/v1/players/{id}/playerVolume/relative"

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
    return(response.text)


def getVolume(id):
    import requests

    url = f"https://api.ws.sonos.com/control/api/v1/players/{id}/playerVolume"

    payload = ""
    headers = {
        'Content-Type': "application/x-www-form-urlencoded",
        'X-Sonos-Api-Key': "536e8328-d836-476b-8035-fd206794c9b7",
        'Host': "api.ws.sonos.com",
        'Authorization': "Bearer d5c9b056-ce40-454f-8423-35cfbb501742",
        'Cache-Control': "no-cache",
        'Postman-Token': "d081d504-024b-42f4-9539-7faa95bc6f1c"
        }


    response = requests.request("GET", url, data=payload, headers=headers)
    print(response.text)
    return(response.text)

def fadeInVol():
    targetVolBathroom = 50
    raiseBy1 = [raiseRelativeVolume(1, bathroom) for _ in range(targetVolBathroom)]
    return(raiseBy1)

async def main():
    # await asyncio.gather(
    #     lowerVolume(),
    #     raiseVolume()
    # )
    targetVolBathroom = 0
    targetVolBedroom = 50
    incrementSize = 2
    currentVolumeBedroom = json.loads(getVolume(bedroom))
    currentVolumeBathroom = json.loads(getVolume(bathroom))
    rangeTotal = targetVolBedroom - currentVolumeBedroom["volume"]
    rangeTotalBathroom = targetVolBathroom - currentVolumeBathroom["volume"]
    numFullIncrementsBedroom = rangeTotal//incrementSize
    numFullIncrementsBathroom = rangeTotalBathroom//incrementSize
    sign = int(np.sign(rangeTotal))
    signBath = int(np.sign(rangeTotalBathroom))




    currentVolumeBathroom = getVolume(bathroom)
    await asyncio.gather(
        *[raiseRelativeVolume(signBath*incrementSize, bathroom) for _ in range(0,abs(numFullIncrementsBathroom))],
        raiseRelativeVolume(rangeTotalBathroom%incrementSize, bathroom),
        *[raiseRelativeVolume(sign*incrementSize, bedroom) for _ in range(0,abs(numFullIncrementsBedroom))],
        raiseRelativeVolume(rangeTotal%incrementSize, bathroom)

    )



asyncio.run(main())
# main()
