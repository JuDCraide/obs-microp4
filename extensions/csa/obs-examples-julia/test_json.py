import json

with open('topology-json/topology_e1.json', 'r') as file:
    data = json.load(file)
    print(data)

topology_e1 = [
    {
        "switchname": "s1",
        "modules": "all",
        "filename": "obs_main",
        "template": "../../templates/common_ethernet_template.up4",
    },
    {
        "switchname": "s2",
        "modules": "ipv4",
        "filename": "obs_main",
        "template": "../../templates/common_ethernet_template.up4",
    },
    {
        "switchname": "s3",
        "modules": "ipv6",
        "filename": "obs_main",
        "template": "../../templates/common_ethernet_template.up4",
    },
]

print(topology_e1)
