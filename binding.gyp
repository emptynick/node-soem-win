{
    "targets" : [
        {
            "target_name": "node-soem",
            "sources" : [ 
                "src/node-soem-master.cc" 
            ],
            "include_dirs" : [ 
                "lib/soem", 
                "lib/osal", 
                "lib/osal/win32",
                "lib/oshw/win32",
                "lib/oshw/win32/wpcap/Include",
                "<!(node -e \"require('nan')\")" 
            ],
            "libraries" : [
                "../lib/build/soem.lib",
                "../lib/oshw/win32/wpcap/Lib/x64/wpcap.lib",
                "../lib/oshw/win32/wpcap/Lib/x64/Packet.lib",
                "winmm.lib",
                "Ws2_32.lib",
                "msvcrt.lib"
            ],
        }
    ]
}
