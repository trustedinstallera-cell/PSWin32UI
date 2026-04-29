class CheckBox : public Button {
public:
    void setChecked(bool checked);
    bool isChecked() const;
    void setTriState(bool enable);  // 是否允许部分选中（Indeterminate）
    CheckState getCheckState() const;  // Unchecked, Checked, Indeterminate
};
