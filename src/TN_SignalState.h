#ifndef TN_SIGNALSTATE_H_
#define TN_SIGNALSTATE_H_

#include <Arduino.h>

enum class TnSignalState
{
    kIdring,        /* 待機状態(default state) */
    kFirstPress,    /* 1回目のシグナルが立ち上がった状態 */
    kFirstHold,     /* 1回目のシグナルが立ち続けている状態 */
    kFirstRelease,  /* 1回目のシグナルが落ちた状態 */
    kSecondIdring,  /* 2回目のシグナルの待機状態 */
    kSecondPress,    /* 2回目のシグナルが立ち上がった状態 */
    kSecondHold,    /* 2回目のシグナルが立ち続けている状態 */
    kSecondRelease, /* 2回目のシグナルが落ちた状態 */
    kThirdIdring,   /* 3回目のシグナルの待機状態 */
    kLongPress,     /* 長時間シグナルが立ち続けてた状態 */
    kLongHold,      /* 長時間シグナルが立ち続けている状態 */
    kLongRelease,   /* 長時間シグナルが立った後、シグナルが落ちた状態 */
    kLowPassFiltering /* ローパスフィルタリング中 */
};

class TN_State; // 前方宣言

class TN_Signal
{
    static const uint32_t kLongPlessTime{500}; // 長押し判定時間
    static const uint32_t kSecondPressTime{200}; // ダブルクリック制限時間 
    static const uint32_t kChatterTime{10}; // チャタリング判定時間

    void Init();

public:
    TN_Signal();
    ~TN_Signal() = default;
    TN_Signal(const TN_Signal &other) = delete;
    TN_Signal &operator=(const TN_Signal &other) = delete;

    TnSignalState GiveSignal(boolean is_high); /* この関数にシグナルを与えて状態を遷移させる */

    // Getter-Setter
    void set_tn_state(TN_State *next) { tn_state_ = next; }
    void set_start_time(uint32_t t) { start_time_ = t; }
    void set_long_press_time(uint32_t t) { long_press_time_ = t; };
    void set_second_press_time(uint32_t t) { second_press_time_ = t; };
    void set_chatter_time(uint32_t t) { chatter_time_ = t; };
    uint32_t start_time() const { return start_time_; }
    uint32_t long_press_time() const { return long_press_time_; }
    uint32_t second_press_time() const { return second_press_time_; }
    uint32_t chatter_time() const { return chatter_time_; }

    // 状態遷移先
    TN_State *Idring() const;
    TN_State *FirstPress() const;
    TN_State *FirstHold() const;
    TN_State *FirstRelease() const;
    TN_State *SecondIdring() const;
    TN_State *SecondPress() const;
    TN_State *SecondHold() const;
    TN_State *SecondRelease() const;
    TN_State *ThirdIdring() const;
    TN_State *LongPress() const;
    TN_State *LongHold() const;
    TN_State *LongRelease() const;

private:
    uint32_t long_press_time_{kLongPlessTime};
    uint32_t second_press_time_{kSecondPressTime};
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
class FirstPressState : public TN_State
{
public:
    FirstPressState() = default;
    ~FirstPressState() = default;

protected:
    void GiveSignal(boolean is_high, TN_Signal *ctx) override;
    TnSignalState GetStateEnum() override { return TnSignalState::kFirstPress; }
};

/* 1回目のシグナルが立ち続けている状態 */
class FirstHoldState : public TN_State
{
public:
    FirstHoldState() = default;
    ~FirstHoldState() = default;

protected:
    void GiveSignal(boolean is_high, TN_Signal *ctx) override;
    TnSignalState GetStateEnum() override { return TnSignalState::kFirstHold; }
};

/* 1回目のシグナルが落ちた状態 */
class FirstReleaseState : public TN_State
{
public:
    FirstReleaseState() = default;
    ~FirstReleaseState() = default;

protected:
    void GiveSignal(boolean is_high, TN_Signal *ctx) override;
    TnSignalState GetStateEnum() override { return TnSignalState::kFirstRelease; }
};

/* 2回目のシグナルの待機状態 */
class SecondIdringState : public TN_State
{
public:
    SecondIdringState() = default;
    ~SecondIdringState() = default;

protected:
    void GiveSignal(boolean is_high, TN_Signal *ctx) override;
    TnSignalState GetStateEnum() override { return TnSignalState::kSecondIdring; }
};

/* 2回目のシグナルが立ち上がった状態 */
class SecondPressState : public TN_State
{
public:
    SecondPressState() = default;
    ~SecondPressState() = default;

protected:
    void GiveSignal(boolean is_high, TN_Signal *ctx) override;
    TnSignalState GetStateEnum() override { return TnSignalState::kSecondPress; }
};

/* 2回目のシグナルが立ち続けている状態 */
class SecondHoldState : public TN_State
{
public:
    SecondHoldState() = default;
    ~SecondHoldState() = default;

protected:
    void GiveSignal(boolean is_high, TN_Signal *ctx) override;
    TnSignalState GetStateEnum() override { return TnSignalState::kSecondHold; }
};

/* 2回目のシグナルが落ちた状態 */
class SecondReleaseState : public TN_State
{
public:
    SecondReleaseState() = default;
    ~SecondReleaseState() = default;

protected:
    void GiveSignal(boolean is_high, TN_Signal *ctx) override;
    TnSignalState GetStateEnum() override { return TnSignalState::kSecondRelease; }
};


/* 3回目のシグナルの待機状態 */
class ThirdIdringState : public TN_State
{
public:
    ThirdIdringState() = default;
    ~ThirdIdringState() = default;

protected:
    void GiveSignal(boolean is_high, TN_Signal *ctx) override;
    TnSignalState GetStateEnum() override { return TnSignalState::kThirdIdring; }
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