#!/usr/bin/env python3
import subprocess
import sys

print(f"Python version: {sys.version}")
print(f"subprocess module: {subprocess.__file__}")

# Test if timeout parameter is supported in Popen
try:
    # Try to create a Popen with timeout parameter
    proc = subprocess.Popen(['echo', 'test'], timeout=1)
    print("✓ Popen timeout parameter is supported")
    proc.wait()
except TypeError as e:
    print(f"✗ Popen timeout parameter error: {e}")
except Exception as e:
    print(f"✗ Other error: {e}")

# Test Popen.__init__ signature
import inspect
popen_signature = inspect.signature(subprocess.Popen.__init__)
print(f"Popen.__init__ parameters: {list(popen_signature.parameters.keys())}")