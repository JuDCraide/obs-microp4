topology = [
    {
        "switchname": "s1",         
        "module": "all",
        "filename": "obs_main",
        "template": "../../templates/common_ethernet_template.up4",
    },
    {
        "switchname": "s2", 
        "module": "ipv4",
        "filename": "obs_main",
        "template": "../../templates/common_ethernet_template.up4",
    },
    {
        "switchname": "s3", 
        "module": "ipv6",
        "filename": "obs_main",
        "template": "../../templates/common_ethernet_template.up4",
    },
]
destination = "./example-julia/distribute_programs.sh" 

f = open(destination, "w")

f.write('echo -e "\\n*********************************"\n')
f.write('echo -e "\\n Generating switch programs with a template "\n')

for switch in topology:
    print(switch)
    line = 'python ../generate_switch_program_w_template.py --switchname {0} --module {1} --filename {2} --template {3}\n'.format(
        switch["switchname"],
         switch["module"],
         switch["filename"],
         switch["template"],
    )
    f.write(line)

f.close()