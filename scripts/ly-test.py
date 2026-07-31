import os, subprocess, sys, platform

class FileNotLeyoError(OSError):
    def __init__(self, message):
        super().__init__(message)
        self.message = message

    def __str__(self):
        return self.message

leyo_dir = os.path.dirname(os.path.abspath(sys.argv[0])).removesuffix('/').removesuffix('/scripts')
test_dir = leyo_dir + '/tests/'

print("\n==== Leyo Test Framework ====\n")


exe: str
if platform.platform() == 'Windows':
    exe = f'{leyo_dir}/bin/leyo.exe'
else:
    exe = f'{leyo_dir}/bin/leyo'

if not os.path.exists(exe):
    raise FileNotFoundError(f'No Leyo Bin at {exe}\nhint: run `make` to generate it')

try:
    result = subprocess.run(
        [exe],
        capture_output=True,
        text=True,
        timeout=5,
    )

    if not (result.returncode == 0 and result.stdout.startswith("Leyo version v")):
        raise FileNotLeyoError('Executable is not Leyo.')

except (OSError, subprocess.TimeoutExpired):
    raise FileNotLeyoError('Executable is not Leyo.')
    
tests: list[str] = []
for found in os.listdir(test_dir):
    full = test_dir + found
    if os.path.isfile(full):
        print(os.path.splitext(found))
        name, ext = os.path.splitext(found)
        if ext == '.leyo':
            print('Found Test: ' + name)
            tests.append(name)  

print(tests)