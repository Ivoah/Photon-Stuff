from PIL import Image
from io import BytesIO
from ScriptingBridge import *
import numpy

import gc
import resource
import objgraph

iTunesSB = SBApplication.applicationWithBundleIdentifier_("com.apple.iTunes")

def image_to_data(image):
    pb = numpy.array(image.convert('RGB')).astype('uint16')
    color = ((pb[:,:,2] >> 3) << 11) | ((pb[:,:,1] >> 2) << 5) | (pb[:,:,0] >> 3)
    return bytes(numpy.dstack(((color >> 8) & 0xFF, color & 0xFF)).flatten().tolist())

err = image_to_data(Image.open('../pictures/err.png'))
#art = iTunesSB.currentTrack().artworks().firstObject().rawData()
def iTunes():
    try:
        track = iTunesSB.currentTrack()
        artworks = track.artworks()
        first = artworks.firstObject()
        data = first.rawData()
        img = Image.open(BytesIO(data)).resize((128, 128), Image.BILINEAR)
        data.release()
        first.release()
        artworks.release()
        track.release()
        gc.collect()
        return image_to_data(img)
    except AttributeError:
        return err

while True:
    iTunes()
    gc.collect()
    print('Memory usage: {:.2f} MB'.format(resource.getrusage(resource.RUSAGE_SELF).ru_maxrss/1000/1000))
