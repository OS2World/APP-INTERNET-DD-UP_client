/* A quick and dirty script to compile the DDUP client */
/* by Erico Mendonca (slaughter@malaconet.org)         */


say 'Enter the path to where you''ll keep the ddup.conf file (no / at the end!):'
pull path

'echo #define file_name ""'||path||'/ddup.conf'
say
say 'ddup.conf will be read from 'path
say
say 'Compiling DDUP...'
'gcc -c ddup.c'
'gcc -c ddup_functions.c'
'gcc -c getopt.c'
'gcc -c getopt1.c'
'gcc -c parse_config.c'
'gcc -c parse_option.c'
'gcc -c update.c'
'gcc -Zexe -O2 -o ddup ddup.o ddup_functions.o getopt.o getopt1.o parse_config.o parse_option.o update.o -lsocket'
say 'Done.'
say 'Do you want to create a configuration file now? (Y/N)'
pull option
if option='Y'|option='y' then do
   'ddup --makeconf'
   say 'Alright, now copy the file ddup.exe somewhere in your path and we''re done.'
   end
     else say 'okay, we''re done now.'

exit
