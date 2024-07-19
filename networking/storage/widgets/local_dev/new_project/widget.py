from mosaico import widget

# Called on widget load
image = widget.createImage()
image.setImage(widget.configAsset("name"))
image.moveTo(2,2)

# Called once each frame
def loop():
    pass
        