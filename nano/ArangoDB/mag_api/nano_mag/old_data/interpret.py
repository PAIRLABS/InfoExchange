

########### Python 2.7 #############
import httplib, urllib, base64
from argparse import ArgumentParser
import json

headers = {
    # Request headers
    'Ocp-Apim-Subscription-Key': 'e91cf8abb8ed4e039fb5dd2d5105f8f2',
}

parser = ArgumentParser()
parser.add_argument("-q", "--query", help="query arg", dest="query", default="")  #required!!!
parser.add_argument("-m", "--model", help="model arg", dest="model", default="")
parser.add_argument("-C", "--complete", help="complete arg", dest="complete", default="0")
parser.add_argument("-c", "--count", help="conut arg", dest="count", default="10")
parser.add_argument("-o", "--offset", help="offset arg", dest="offset", default="0")
parser.add_argument("-t", "--timeout", help="timeout arg", dest="timeout", default="1000")
args = parser.parse_args()


params = urllib.urlencode({
    # Request parameters
    'query': 'papers by jaime',
    'complete': '1',
    'count': 1,
    'offset': '0',
    'timeout': '',
    'model': 'latest',
})

try:
    conn = httplib.HTTPSConnection('api.labs.cognitive.microsoft.com')
    conn.request("GET", "/academic/v1.0/interpret?%s" % params, "{body}", headers)
    response = conn.getresponse()
    data = response.read()
    #print(data)

    json_data = json.loads(data);
    print (json_data['interpretations'][0]['rules'][0]['output']['value'].encode('UTF-8','ignore'))  #success
    #print len(json_data['interpretations'])
    #print json_data['interpretations'][1]['logprob']
    #print type(json_data['interpretations'][0]['rules'][0]['output']['value'])

    conn.close()
except Exception as e:
    print("[Errno {0}] {1}".format(e.errno, e.strerror))




######### evaluate  #########################
"""
params2 = urllib.urlencode({
    # Request parameters    
    'expr': json_data['interpretations'][0]['rules'][0]['output']['value'],
    'model': 'latest',
    'count': '3',
    'offset': '0',
    'orderby': '',
    'attributes': 'Id,Ti,AA.AuN,Y',
})

try:
    conn2 = httplib.HTTPSConnection('api.labs.cognitive.microsoft.com')
    conn2.request("GET", "/academic/v1.0/evaluate?%s" % params2, "{body}", headers)
    response2 = conn2.getresponse()
    data2 = response2.read()
    #print(data2)
    json_data2 = json.loads(data2);
    print json_data2['entities'][0]
    conn2.close()
except Exception as e:
    print("[Errno {0}] {1}".format(e.errno, e.strerror))


"""