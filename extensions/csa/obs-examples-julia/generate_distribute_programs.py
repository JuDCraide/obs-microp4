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
destination = "./example-julia/generated_distribute_programs.sh"
allModules = ["ipv4", "ipv6"]
modules = set()

f = open(destination, "w")

f.write('sudo mn -c\n')
f.write('\necho -e "\\n*********************************"\n')
f.write('echo -e "\\n Generating switch programs with a template "\n')

for switch in topology:
    line = 'python ../generate_switch_program_w_template.py --switchname {0} --module {1} --filename {2} --template {3}\n'.format(
        switch["switchname"],
        switch["module"],
        switch["filename"],
        switch["template"],
    )
    f.write(line)
    switchModules = switch["module"].split(",")
    switch["modulesParsed"] = switchModules
    for module in switchModules:
        if (module == "all"):
            modules.update(allModules)
            switch["modulesParsed"] = allModules
            continue
        else:
            modules.add(module)

f.write('\necho -e "\\n*********************************"\n')
f.write('echo -e "\\n Compiling uP4 includes"\n')
for module in modules:
    fixedPart = "${UP4ROOT}/build/p4c-msa -I ${UP4ROOT}/build/p4include"
    line = '{0} -o {1}.json {2}.up4\n'.format(fixedPart, module, module)
    f.write(line)

f.write('\necho -e "\\n*********************************"\n')
f.write('echo -e "\\n Compiling uP4 main programs"\n')
for switch in topology:
    submodules = ""
    for i, module in enumerate(switch["modulesParsed"]):
        submodules += module + ".json"
        if i < len(switch["modulesParsed"])-1:
            submodules += ", "
        else: submodules += " "
    p4cMsa = "${UP4ROOT}/build/p4c-msa"
    p4include = "${UP4ROOT}/build/p4include"
    line = '{0} --target-arch v1model -I {1}  -l {2}{3}_all_main.up4\n'.format(
        p4cMsa, p4include, submodules, switch["switchname"])
    f.write(line)

f.write('\necho -e "\\n*********************************"\n')
f.write('echo -e "\\n Compiling P4 programs "\n')
for switch in topology:
    line = '../../p4c-compile.sh {0}_all_main_v1model.p4\n'.format(switch["switchname"])
    f.write(line)

f.write('\nbold=$(tput bold)\n')
f.write('normal=$(tput sgr0)\n')

f.write('\nBMV2_MININET_PATH=${UP4ROOT}/extensions/csa/obs-examples/simple-topo\n')
f.write('BMV2_SIMPLE_SWITCH_BIN=${UP4ROOT}/extensions/csa/msa-examples/bmv2/targets/simple_switch/simple_switch\n')

f.write('\nP4_MININET_PATH=${UP4ROOT}/extensions/csa/msa-examples/bmv2/mininet\n')

f.write('\necho -e "${bold}\\n*********************************"\n')
f.write('echo -e "Running Tutorial program: obs_example_v1model${normal}"\n')
f.write('sudo bash -c "export P4_MININET_PATH=${P4_MININET_PATH} ;  \\ \n')
f.write('  $BMV2_MININET_PATH/obs_simple_topo_v1model_sw.py --behavioral-exe $BMV2_SIMPLE_SWITCH_BIN \\ \n')
f.write('  --num-hosts 4 ')
for i, switch in enumerate(topology):
    line = '--json{0} ./{1}_{2}_main_v1model.json '.format( i+1, switch["filename"], switch["module"])
    f.write(line)
f.write('"\n')

f.write('echo -e "\\n*********************************\\n${normal}"\n')
f.close()
