import numpy
import base64
import threading
from bottle import *
from PIL import Image
from io import BytesIO

from selenium import webdriver
from selenium.webdriver.support.ui import WebDriverWait

def image_to_data(image):
    pb = numpy.array(image.convert('RGB')).astype('uint16')
    color = ((pb[:,:,2] >> 3) << 11) | ((pb[:,:,1] >> 2) << 5) | (pb[:,:,0] >> 3)
    return bytes(numpy.dstack(((color >> 8) & 0xFF, color & 0xFF)).flatten().tolist())

options = webdriver.ChromeOptions()
options.add_argument('headless')
chrome = webdriver.Chrome(chrome_options = options)

try:
    wait = WebDriverWait(chrome, 60*5)

    chrome.get('http://make.girls.moe')

    btn = chrome.find_element_by_css_selector('.generator > button')

    print('Loading model...')
    wait.until(lambda _: btn.is_enabled())
    print('Done')
    btn.click()
    wait.until(lambda _: btn.is_enabled())
    img = chrome.find_element_by_css_selector('.result-canvas > img')

    moes = []

    loading = False

    @get('/tft')
    def get_moe():
        while not moes: pass
        moe = moes.pop()
        print(len(moes))
        if not loading: threading.Thread(None, gen_moes).start()
        return image_to_data(Image.open(BytesIO(moe)))

    @get('/png')
    def get_png():
        response.content_type = 'image/png'
        while not moes: pass
        moe = moes.pop()
        print(len(moes))
        if not loading: threading.Thread(None, gen_moes).start()
        return moe

    def gen_moes():
        global loading
        loading = True
        while len(moes) < 25:
            btn.click()
            wait.until(lambda _: btn.is_enabled())
            moes.append(base64.b64decode(img.get_attribute('src')[22:]))
            print(len(moes))

        loading = False

    threading.Thread(None, gen_moes).start()
    run(host = '0.0.0.0', port = 1337)
finally:
    chrome.close()
