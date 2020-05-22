# build time configuration.
# select your favourite bloat here.

# if you have freetype2 and png16, you can uncomment this:
# VKDT_USE_FREETYPE=1
# export VKDT_USE_FREETYPE

# if you disable this, the i-raw module will not be built
# and you will be unable to load raw images.
# since rawspeed is a submodule this is only useful if
# you want to avoid the recursive dependencies.
VKDT_USE_RAWSPEED=1
export VKDT_USE_RAWSPEED

# exiv2 can optionally be used to load some super basic metadata (iso speed,
# shutter time etc) inside the i-raw module. that is, you don't have that you
# don't need to disable this.
# VKDT_USE_EXIV2=1
# export VKDT_USE_EXIV2

CC=clang
CXX=clang++
export CC CXX
