from datetime import date
from mosaico import widget

# Get the current date
today = date.today()
d = today.strftime("%d")
m = today.strftime("%m")
y = today.strftime("%Y")

# Display on matrix
day_month = widget.createText()
day_month.setText(d + "-" + m)
year = widget.createText()
year.setText(y)
year.translateY(8)

# Nothing to do in the loop ;)
def loop():
    pass