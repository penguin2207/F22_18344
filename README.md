# F22_18344
Eli Wirth-Apley (ewirthap)
Chalisa Udompanyawit (cudompan)

Two versions of our cache simulator are included, cache.cpp is our simple cache which includes support for simulating up to task 2. cache_2Level.cpp and similar _2Level files includes support for up to task 4. Either version can be selected. To run them, edit the run.py script with appropriate output folders. This will simulate various cache sizes and configurations. Included as example outputs of our two level cache simulator are pinOut_4096.csv, pinOut_16384.csv and pinOut_262144.csv. These can be used by our iterator tool.

Our automatic Destiny-running tool is runDestiny2.py. It can be run by changing the cacheSize variable to desired cache size simulation and optionally changing how many block sizes it iterates over. 128 is the maximum lines it seems to be able to handle. It outputs to destinyLog.csv, which already has a library of simulated cache configurations. 

Our iteration tool uses the output of the above two programs to generate an iterationLog.csv which outputs data in the form of [tuple, AMAT, area]. The tuple is formatted as specified in the task 4 description. To run, simply specify which pinOut to use by changing pinFile, optionally change spread and ensure destinyLog.csv is in the same directory as the iteration tool. Lower spread values outputs more data. Higher spread, means increased filtering. Depending on the size of the pinOut file being used, too much data can be hard to parse.
