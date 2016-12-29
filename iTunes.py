import numpy
import bottle
from PIL import Image
from io import BytesIO
from ScriptingBridge import *

iTunesSB = SBApplication.applicationWithBundleIdentifier_("com.apple.iTunes")

def image_to_data(image):
    pb = numpy.array(image.convert('RGB')).astype('uint16')
    color = ((pb[:,:,2] >> 3) << 11) | ((pb[:,:,1] >> 2) << 5) | (pb[:,:,0] >> 3)
    return bytes(numpy.dstack(((color >> 8) & 0xFF, color & 0xFF)).flatten().tolist())

@bottle.get('/volumeUp')
def volumeUp():
    iTunesSB.setSoundVolume_(iTunesSB.soundVolume() + 1)

@bottle.get('/volumeDown')
def volumeDown():
    iTunesSB.setSoundVolume_(iTunesSB.soundVolume() - 1)

@bottle.get('/previous')
def previous():
    iTunesSB.previousTrack()

@bottle.get('/next')
def next():
    iTunesSB.nextTrack()

@bottle.get('/playPause')
def playPause():
    iTunesSB.playpause()

err = image_to_data(Image.open('err.png'))
@bottle.get('/artwork')
def iTunes():
    try:
        art = iTunesSB.currentTrack().artworks().firstObject().rawData()
        img = Image.open(BytesIO(art)).resize((128, 128), Image.BILINEAR)
        return image_to_data(img)
    except AttributeError:
        return err

@bottle.get('/title')
def title():
    return iTunesSB.currentTrack().name()

@bottle.get('/artist')
def title():
    return iTunesSB.currentTrack().artist()

@bottle.get('/album')
def title():
    return iTunesSB.currentTrack().album()

bottle.run(reloader = True, host = '0.0.0.0', port = 1337)
