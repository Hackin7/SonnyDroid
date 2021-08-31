from PIL import Image

f = open("/tmp/screen_data")

h = 240
w = 320
img = Image.new('RGB', (w, h), (255,255,255))
pixels = img.load()
for j in range(h):
    for i in range(w):
        data = ['0'+f.readline()]*4 #.split(",")
        colour = int(data[1])//(256*256),int(data[2])//256%256,int(data[3])%256
        pixels[i,j] = colour


f.close()
#img.show()

img.save('/tmp/img.png')
