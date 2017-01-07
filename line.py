import requests,json
LINE_ACCESS_TOKEN="32xcreVbSqhuoOfPGfyIhji81k7PtKjnOmr1tz99h4L" #token
url = "https://notify-api.line.me/api/notify"
msg = {"message":"Test Message"} #message
LINE_HEADERS = {'Content-Type':'application/x-www-form-urlencoded',"Authorization":"Bearer "+LINE_ACCESS_TOKEN}
session = requests.Session()
resp =session.post(url, headers=LINE_HEADERS, data=msg)
print(resp.text)