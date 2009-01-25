////////////////////////////////////////
//  File      : sysicon.cpp           //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include "sysicon.h"

////////////////////////////////////////

Sysicon::Sysicon(Frame *frame, QWidget *parent) : QWidget(parent)
{
    frm = frame;
    read_settings();
    setContentsMargins(0, 0, 0, 0);
    setFixedSize(32, 32);
    setToolTip(frm->cl_name());
}

Sysicon::~Sysicon()
{}

void Sysicon::read_settings()
{
    // get style path
    QSettings *antico = new QSettings(QCoreApplication::applicationDirPath() + "/antico.cfg", QSettings::IniFormat, this);
    antico->beginGroup("Style");
    QString stl_name = antico->value("name").toString();
    QString stl_path = antico->value("path").toString();
    antico->endGroup(); //Style
    // get style values
    style = new QSettings(stl_path +stl_name, QSettings::IniFormat, this);
    style->beginGroup("Sysicon");
    s_icon_pix = stl_path + style->value("s_icon_pix").toString();
    pix = QPixmap(s_icon_pix);
    style->endGroup(); //Sysicon
    style->beginGroup("Other");
    close_dock_pix = stl_path + style->value("close_dock_pix").toString();
    style->endGroup(); //Other
}

void Sysicon::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
 // painter.drawRoundRect(0, 0, width(), height(), 5, 20);
    painter.drawPixmap(QRect(0, 0, width(), height()), pix, QRect(0, 0, pix.width(), pix.height()));// sysicon pixmap
    painter.drawPixmap(QRect(3, 3, height()-6, height()-6), frm->cl_icon(), QRect(0, 0, frm->cl_icon().width(), frm->cl_icon().height()));// sysicon
}

void Sysicon::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit show_sys(this);
        frm->raise();
        close();
    }
    if (event->button() == Qt::RightButton)
    {
        QMenu *menu = new QMenu(this);
        QAction *sys_close = menu->addAction(QIcon(close_dock_pix), "Close");
        menu->popup(event->globalPos());
        connect(sys_close, SIGNAL(triggered()), this, SLOT(sys_close()));
    }
}

void Sysicon::sys_close()
{
    emit destroy_sys(this);
    frm->destroy();
    close();
}

void Sysicon::update_style()
{
    read_settings();
    update();
}

