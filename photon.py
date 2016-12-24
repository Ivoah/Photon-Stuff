import bottle
from PIL import Image
import numpy
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

err = image_to_data(Image.open('../pictures/err.png'))
@bottle.get('/iTunes')
def iTunes():
    try:
        art = iTunesSB.currentTrack().artworks().firstObject().rawData()
        img = Image.open(BytesIO(art)).resize((128, 128), Image.BILINEAR)
        return image_to_data(img)
    except AttributeError:
        return err

frame = None
last_img = None
@bottle.get('/<image>')
def get_image(image):
    global frame, last_img
    if image != last_img:
        frame = 0
    last_img = image
    img = Image.open('../pictures/{}'.format(image))
    if img.format == 'GIF':
        img.seek(frame)
        frame += 1
        if frame >= img.n_frames: frame = 0
    return image_to_data(img.resize((128, 128), Image.BILINEAR))

bottle.run(reloader = True, host = '0.0.0.0', port = 1337)
