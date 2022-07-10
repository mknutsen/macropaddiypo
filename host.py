from serial import Serial
from mysequencer.sequencer import Sequencer
from mysequencer.clock import Clock
from mysequencer.track import Track

from mido import get_input_names, get_output_names, open_input, open_output, Message
import logging
from threading import Thread

GLOBAL_OUTPUT_PORT = None
TRACK = None
THREAD = None
SEQUENCER = None
_PAD_SERIAL_PORT = "/dev/tty.usbmodem2301"
_MIDI_OUTPUT_PORT_NAME = "mio"
_DELETE = False
class HostException(Exception):
    """ExceptionClass"""


class InvalidCommand(HostException):
    """Invalid step"""

def delete():
    global TRACK
    if not TRACK:
        return

    TRACK.delete()

    _DELETE = True


def main():
    global TRACK, GLOBAL_OUTPUT_PORT, THREAD, SEQUENCER
    beats_per_minute = 120
    beat_length = 8
    midi_channel = 0  # midi indexing is off by one
    
    clock = Clock(beats_per_minute)

    try:
        GLOBAL_OUTPUT_PORT = open_output(_MIDI_OUTPUT_PORT_NAME)
    except:
        print(get_output_names())
        raise

    # uses the clock to handle the sequence and sends notes to the track
    SEQUENCER = Sequencer(beats_per_minute=beats_per_minute, beat_length=beat_length, clock=clock)

    # outputs notes to midi channel
    TRACK = Track(sequence=SEQUENCER, midi_channel=midi_channel, port=GLOBAL_OUTPUT_PORT)

    # reads for input from the pad
    THREAD = Thread(target=read)
    THREAD.start()

def write_note(button_pressed):
    global SEQUENCER
    SEQUENCER.add_note(button_pressed + 50)

if __name__ == "__main__":

    logging.basicConfig(filename="example.log", encoding="utf-8", level=logging.DEBUG)
    try:
        main()
    finally:
        delete()

def read():
    global _DELETE
    with Serial(port=_PAD_SERIAL_PORT, baudrate=115200) as ser:
        while not _DELETE:
            bs = ser.read_until().decode('utf-8')
            logging.error(f"Incoming string: {repr(bs)}")
            
            auth, command, value = bs.split("-")
            if auth != "abc123":
                raise InvalidCommand()

            if command != "p":
                raise InvalidCommand()
            
            write_note(value)

            

