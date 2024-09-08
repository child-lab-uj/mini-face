import pathlib
import sys
sys.path.append(str(pathlib.Path(__file__).parent.parent.absolute() / 'build'))
print(*sys.path, sep=',\n')
import GazeTracking
