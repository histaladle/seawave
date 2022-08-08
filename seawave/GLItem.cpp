#include "GLItem.h"
GLItem::GLItem()
{
}

void GLItem
::draw(GLDrawConfig config)
{
    Q_UNUSED(config);
    if(!vBuffer.isCreated())
    {
        return;
    }
    if(!inited)
    {
        return;
    }
}

void GLItem::updateBuffer()
{
}

void GLItem::setLight(int index, LightProperty light)
{
    Q_UNUSED(index);
    Q_UNUSED(light);
}

void GLItem::renderDepth(GLDrawConfig config)
{
    Q_UNUSED(config);
}

void GLItem::updateFrame()
{

}

GLItem::~GLItem()
{

}

