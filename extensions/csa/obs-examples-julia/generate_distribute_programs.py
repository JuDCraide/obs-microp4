topology = [
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
destination = "./example-julia/distribute_programs.sh" 

f = open(destination, "w")

f.write('echo -e "\\n*********************************"')

f.close()