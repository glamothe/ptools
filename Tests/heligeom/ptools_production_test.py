from  os import system, popen
import commands
#compteur
nb =0 
tot =0

#list of test
l = ["generate_B_DNA.py","basicManipulation.py","CGfromPDBFile.py","bench_ener.py"]

#test frame
for name in l:
    shortname = name.split(".")[0]
    #execute test
    system("python "+name+" >"+ shortname+".result")
    #compare result
    result = commands.getoutput("diff "+ shortname+".result "+ shortname+".expected" )
    
    if not result: 
        print name,"ok"
        nb+=1
        tot+=1
    else: 
        print name,"fail"
        tot+=1

print "success:",nb, "/",tot