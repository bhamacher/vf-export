# vf-export
C++ Python Adapter for PythonConverter

For more details about pyhton-converter visit: https://github.com/ZeraGmbH/PythonConverter


## Vein Interface

### Components

PAR_InputPath: The db file path
PAR_OutputPath: the path where we want to generate the xml file
PAR_Session: the session to convert
PAR_Engine: the engine to use. eg MTVisRes.py or MTVisMain.py
Status: true if python-converter is connected and false if modules where not found.

If the modules where not found follwoing steps are necessary :
1. kill process
2. adapt PYTHONPATH
3. restart process

### RPCS

RPC_Convert: Does the actual conversion. 

Returns true on success or false on failure.
