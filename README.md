Simple longest prefix match application
==============

Simple application that accepts input file with definitions of IP addresses and corresponding mapping to ASN numbers. This is file is loaded on the application start and used for building trie. Trie is structure that is then used for searching using longest prefix match algorithm. 

Application also accepts second argument with file that is filled by IP addresses that are required to translate.

# Usage
Run as:
```
./lpm -i asns.txt <ip.txt
```

File asns.txt is defined as follows:
```
1.0.0.0/24 15169
1.0.128.0/18 9737
1::1/128 42298
1.0.160.0/19 9737
```

File ip.txt is defined as follows:
```
178.215.97.139
88.135.226.247
130.37.34.210
176.43.82.172
83.59.131.229
2001:0:5ef5:79fb:83c:14f1:4370:ebbc
201.76.218.142
78.56.152.16
```

# Building
```
make            compile project - release version
make pack       packs all required files to compile this project    
make clean      clean temp compilers files    
make clean-all  clean all compilers files - includes project    
make clean-outp clean output project files 
```

# Features
- IPv4 and IPv6 support
- optimized for fast processing
