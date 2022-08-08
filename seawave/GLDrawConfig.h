#ifndef GLDRAWCONFIG_H
#define GLDRAWCONFIG_H
#include <QMatrix4x4>
struct GLDrawConfig
{
    QMatrix4x4 projection;
    QMatrix4x4 view;
    QMatrix4x4 model;
};
#endif // GLDRAWCONFIG_H
