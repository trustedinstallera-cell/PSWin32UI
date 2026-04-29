class RadioButton : public Button {
public:
    void setChecked(bool checked);
    bool isChecked() const;
    void setGroup(int groupId);  // 同一组内互斥
};