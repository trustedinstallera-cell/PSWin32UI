class Button : public Control {
public:
    void setAutoSize(bool enable);
    bool isAutoSize() const { return m_autoSize; }
    
protected:
    virtual Size getPreferredSize() {
        // 获取按钮文本的宽度和高度（用 DC 计算）
        HDC hdc = GetDC(m_hwnd);
        HFONT oldFont = (HFONT)SelectObject(hdc, getFont());
        RECT rect = {0, 0, 0, 0};
        DrawText(hdc, getText().c_str(), -1, &rect, DT_CALCRECT);
        SelectObject(hdc, oldFont);
        ReleaseDC(m_hwnd, hdc);
        
        // 加上内边距（根据主题）
        return Size(rect.right - rect.left + 10, rect.bottom - rect.top + 6);
    }
    
    void updateSize() {
        if (m_autoSize) {
            Size pref = getPreferredSize();
            setSize(pref);
        }
    }
};

/*
优先级 控件 原因
🔴 高 Static（标签） 文本长度变化频繁
🔴 高 Button 文本可能是多语言，长度变化大
🔴 高 Edit（单行输入框） 输入内容长度变化
🟡 中 ComboBox 下拉项文本变化
🟡 中 CheckBox / RadioButton 右侧文本可能变化
⚪ 低 GroupBox 标题通常固定
⚪ 低 ListView / TreeView 通常不需要 autosize，由用户手动拖拽调整

*/