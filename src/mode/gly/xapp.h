#pragma once

#include "sk_app/Application.h"
#include "sk_app/Window.h"
#include "XWindow.h"

// ȫ�ֵ�app����
#define xApp ((sia::platy::XApp*)_s_xapp)

// paintman ���ڴ������ļ���������paint����
// ������ʵ������ʽ�Ļ���

namespace sia {
namespace graphy {
class PaintMan;
}
namespace platy {

extern sia::platy::XApp* _s_xapp;

class SIA_API XWindow;
class SIA_API XApp : public sk_app::Application {
public:
    XApp();
    ~XApp();

    void setMainWindow(XWindow* w);

    XWindow* mainWindow()const {
        return mainwin_;
    }

    sk_app::Window::BackendType backendType()const {
        return backend_;
    }

    virtual void onIdle() override;

    void loadPaintConfig(const SkString& cfgpath);
protected:
    virtual void doCreate(int argc, char** argv, void* platformData) {
    }

private:
    XWindow* mainwin_ = nullptr;
    sk_app::Window::BackendType backend_ = sk_app::Window::kNativeGL_BackendType;
    my_sp<graphy::PaintMan> paintman_;
};
}
}
