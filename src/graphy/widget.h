#pragma once

#include "siapi.h"
#include "SkView.h"
#include "paintgroup.h"
#include "event.h"

// Widget �ǿ��ӻ��������С��λ
// ���е�widget����Scene�й���
namespace sia {
namespace graphy {

class Layout;
class GRAPHY_API Widget :
    public SkView {

    friend class Scene;
public:
    Widget(const SkString& name = SkString(), Widget* parent = nullptr);
    virtual ~Widget();

    // ���ĸ��׽ڵ�
    void setParent(Widget* parent);

    inline Widget* parent()const {
        return parent_;
    }

    SkRect rect()const;

    /*! \brief �ڸ�����������λ��
     *
     */
    SkRect geometry()const;

    /*! \brief ��scene�е�λ��
     *
     */
    SkRect windowGeometry()const;

    /*! \brief ��������ת����widget������
     *
     *    @param    pt ���ڣ�scene������
     *    @return   widget����
     */
    SkPoint windowToClient(SkPoint pt) const;

    /*! \brief widget����ת��Ϊwindow��scene������
     *
     *    @param    pt widget����
     *    @return   ���ڣ�scene������
     */
    SkPoint clientToWindow(SkPoint pt) const;

    inline SkPoint pos()const {
        return pos_;
    }

    void setGeometry(const SkRect& r);

    void moveTo(SkScalar x, SkScalar y);

    void setZIndex(int16_t);

    void addChild(Widget* w);

    void setLayout(Layout* l);

    /*! \brief ����������һ�β���
     *
     *    @param
     *    @return
     */
    void relayout();

    inline SkString name()const {
        return name_;
    }

    void setName(const SkString& name);

    void forEachChild(std::function<bool(Widget*)> onChild)const;

    std::vector<Widget*> children()const {
        return children_;
    }

    inline int16_t zindex()const {
        return z_index_;
    }

    /*! \brief �Ƴ�����
     *
     *    @param	ch ���ӽڵ�
     *    @return
     */
    void removeChild(Widget* ch);

    Widget* root() const;

    /*! \brief ����zindex �仯��ʱ�����
     *
     */
    void onChildZIndexChange(Widget* w);

    /*! \brief �����еĺ��ӽڵ�㲥�¼�
     *
     *    @param	evt	��Ҫ�㲥���¼�
     *    @return
     */
    void broadcast(const SkEvent& evt);

protected:
    virtual void onDestroy();
    virtual void onSizeChange() override;
    virtual void onLayoutChange() {
    }

    virtual void onDraw(SkCanvas* canvas) override;

    // ����λ���ҵ����µ�Widget
    virtual Widget* hitTest(int x, int y);

    // �����ص�UI����
    virtual void onAttach() { }

    virtual bool onKey(sia::KeyBoard key, sia::InputState state, uint32_t modifiers) {
        return false;
    }

    virtual bool onMouse(int x, int y, sia::InputState state, uint32_t modifiers) {
        return false;
    }

    virtual bool onMouseWheel(float delta, uint32_t modifiers) {
        return false;
    }

protected:
    // internals
    bool onMouse_internal(int x, int y, InputState state, uint32_t modifiers);

    bool onMouseWheel_internal(float delta, uint32_t modifiers);

private:
    // �����Ӱ���z_index������������
    void sortChildren();

    // �Ƿ���ں���
    bool childExist(Widget* w)const;

private:
    // ���Ͻǵ�λ�ã��������������parent widget
    SkPoint pos_;

    // ���׽ڵ㣬����Ϊ��
    Widget* parent_ = nullptr;

    // ���ֹ���
    Layout* layout_ = nullptr;

    // ���ӽڵ�
    std::vector<Widget*> children_;

    // z ��˳��
    int16_t z_index_ = 0;

    // ������
    SkString name_;
};

}
}
