#ifndef TN_SIGNALSTATE_H_
#define TN_SIGNALSTATE_H_

#include <Arduino.h>

enum class TnSignalState
{
    kIdring,        /* 待機状態(default state) */
    kSinglePress,   /* 1回目のシグナルが立ち上がった状態 */
    kSingleHold,    /* 1回目のシグナルが立ち続けている状態 */
    kSingleRelease, /* 1回目のシグナルが落ちた状態 */
    kDoubleIdring,  /* 2回目のシグナルの待機状態 */
    kDoublePress,   /* 2回目のシグナルが立ち上がった状態 */
    kDoubleHold,    /* 2回目のシグナルが立ち続けている状態 */
    kDoubleRelease, /* 2回目のシグナルが落ちた状態 */
    kLongPress,     /* 長時間シグナルが立ち続けてた状態 */
    kLongHold,      /* 長時間シグナルが立ち続けている状態 */
    kLongRelease,   /* 長時間シグナルが立った後、シグナルが落ちた状態 */
    kLowPassFiltering /* ローパスフィルタリング中 */
};

class TN_State;

/**
 * Class TN_Signal
**/
class TN_Signal
{
    static const uint32_t kLongPlessTime{500}; 
    static const uint32_t kDoublePressTime{200}; 
    static const uint32_t kChatterTime{10};

    void Init();

public:
    TN_Signal();
    // TN_Signal(boolean is_toggle);
    ~TN_Signal() = default;
    TN_Signal(const TN_Signal &other) = delete;
    TN_Signal &operator=(const TN_Signal &other) = delete;

    TnSignalState GiveSignal(boolean is_high); /* この関数にシグナルを与えて状態を遷移させる */

    // Getter-Setter
    void set_tn_state(TN_State *next) { tn_state_ = next; }
    void set_start_time(uint32_t t) { start_time_ = t; }
    void set_long_press_time(uint32_t t) { long_press_time_ = t; };
    void set_double_press_time(uint32_t t) { double_press_time_ = t; };
    void set_chatter_time(uint32_t t) { chatter_time_ = t; };
    uint32_t start_time() const { return start_time_; }
    uint32_t long_press_time() const { return long_press_time_; }
    uint32_t double_press_time() const { return double_press_time_; }
    uint32_t chatter_time() const { return chatter_time_; }
//    boolean is_toggle() const { return is_toggle_; }

    // 状態遷移先
    TN_State *Idring() const;
    TN_State *SinglePress() const;
    TN_State *SingleHold() const;
    TN_State *SingleRelease() const;
    TN_State *DoubleIdring() const;
    TN_State *DoublePress() const;
    TN_State *DoubleHold() const;
    TN_State *DoubleRelease() const;
    TN_State *LongPress() const;
    TN_State *LongHold() const;
    TN_State *LongRelease() const;

private:
//    boolean is_toggle_{false};
    uint32_t long_press_time_{kLongPlessTime};
    uint32_t double_press_time_{kDoublePressTime};
    uint32_t chatter_time_{kChatterTime};
    uint32_t receipt_time_{0};
    uint32_t start_time_{0};
    TN_State *tn_state_;
};

/* ---------------------------------------------------------------------- */

class TN_State
{
    friend class TN_Signal;

public:
    TN_State();
    virtual ~TN_State();

protected:
    virtual void GiveSignal(boolean is_high, TN_Signal *ctx) = 0;
    virtual TnSignalState GetStateEnum() = 0;
};

/* ---------------------------------------------------------------------- */

/* 待機状態(default state) */
class IdringState : public TN_State
{
public:
    IdringState() = default;
    ~IdringState() = default;

protected:
    void GiveSignal(boolean is_high, TN_Signal *ctx) override;
    TnSignalState GetStateEnum() override { return TnSignalState::kIdring; }
};

/* 1回目のシグナルが立ち上がった状態 */
class SinglePressState : public TN_State
{
public:
    SinglePressState() = default;
    ~SinglePressState() = default;

protected:
    void GiveSignal(boolean is_high, TN_Signal *ctx) override;
    TnSignalState GetStateEnum() override { return TnSignalState::kSinglePress; }
};

/* 1回目のシグナルが立ち続けている状態 */
class SingleHoldState : public TN_State
{
public:
    SingleHoldState() = default;
    ~SingleHoldState() = default;

protected:
    void GiveSignal(boolean is_high, TN_Signal *ctx) override;
    TnSignalState GetStateEnum() override { return TnSignalState::kSingleHold; }
};

/* 1回目のシグナルが落ちた状態 */
class SingleReleaseState : public TN_State
{
public:
    SingleReleaseState() = default;
    ~SingleReleaseState() = default;

protected:
    void GiveSignal(boolean is_high, TN_Signal *ctx) override;
    TnSignalState GetStateEnum() override { return TnSignalState::kSingleRelease; }
};

/* 2回目のシグナルの待機状態 */
class DoubleIdringState : public TN_State
{
public:
    DoubleIdringState() = default;
    ~DoubleIdringState() = default;

protected:
    void GiveSignal(boolean is_high, TN_Signal *ctx) override;
    TnSignalState GetStateEnum() override { return TnSignalState::kDoubleIdring; }
};

/* 2回目のシグナルが立ち上がった状態 */
class DoublePressState : public TN_State
{
public:
    DoublePressState() = default;
    ~DoublePressState() = default;

protected:
    void GiveSignal(boolean is_high, TN_Signal *ctx) override;
    TnSignalState GetStateEnum() override { return TnSignalState::kDoublePress; }
};

/* 2回目のシグナルが立ち続けている状態 */
class DoubleHoldState : public TN_State
{
public:
    DoubleHoldState() = default;
    ~DoubleHoldState() = default;

protected:
    void GiveSignal(boolean is_high, TN_Signal *ctx) override;
    TnSignalState GetStateEnum() override { return TnSignalState::kDoubleHold; }
};

/* 2回目のシグナルが落ちた状態 */
class DoubleReleaseState : public TN_State
{
public:
    DoubleReleaseState() = default;
    ~DoubleReleaseState() = default;

protected:
    void GiveSignal(boolean is_high, TN_Signal *ctx) override;
    TnSignalState GetStateEnum() override { return TnSignalState::kDoubleRelease; }
};

/* 長時間シグナルが立ち続けてた状態 */
class LongPressState : public TN_State
{
public:
    LongPressState() = default;
    ~LongPressState() = default;

protected:
    void GiveSignal(boolean is_high, TN_Signal *ctx) override;
    TnSignalState GetStateEnum() override { return TnSignalState::kLongPress; }
};

/* 長時間シグナルが立ち続けている状態 */
class LongHoldState : public TN_State
{
public:
    LongHoldState() = default;
    ~LongHoldState() = default;

protected:
    void GiveSignal(boolean is_high, TN_Signal *ctx) override;
    TnSignalState GetStateEnum() override { return TnSignalState::kLongHold; }
};

/* 長時間シグナルが立った後、シグナルが落ちた状態 */
class LongReleaseState : public TN_State
{
public:
    LongReleaseState() = default;
    ~LongReleaseState() = default;

protected:
    void GiveSignal(boolean is_high, TN_Signal *ctx) override;
    TnSignalState GetStateEnum() override { return TnSignalState::kLongRelease; }
};


#endif // TN_SIGNALSTATE_H_