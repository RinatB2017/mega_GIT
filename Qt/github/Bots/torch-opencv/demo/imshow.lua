local cv = require 'cv'
require 'cv.imgcodecs'
require 'cv.highgui'

if not arg[1] then
    print('Usage: `th demo/imshow.lua path-to-image`')
    print('Now using demo/data/lena.jpg')
end

local im = cv.imread {arg[1] or 'demo/data/lena.jpg', cv.IMREAD_GRAYSCALE}
if im:nDimension() == 0 then
    print('Problem loading image\n')
    os.exit(0)
end

cv.imshow {"Hello, Lua!", im}
cv.waitKey {0}
