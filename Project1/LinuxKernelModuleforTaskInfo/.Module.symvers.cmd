cmd_/home/orange/Desktop/OS/Module.symvers := sed 's/\.ko$$/\.o/' /home/orange/Desktop/OS/modules.order | scripts/mod/modpost -m -a  -o /home/orange/Desktop/OS/Module.symvers -e -i Module.symvers   -T -