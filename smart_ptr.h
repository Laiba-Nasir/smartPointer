#ifndef SP
#define SP
using namespace std;
#include <iostream>
#include <exception>

class null_ptr_exception : public exception {
private:
    string exception = "error ";
public:
    null_ptr_exception(string exception = "error ") {
        //cout << "\n******* In null_ptr_exception constructor *************\n";
        this->exception = exception;
    }
};

template <typename T>
class smart_ptr {
private:
    T* ptr_;
    int* ref_;
    // TO BE PRIVATE
    void print(string msg = "MSG") const; // DONE
public:
    // make empty SP, ptr and ref to nullptr 
    smart_ptr(); // DONE

    // make smart pointer point to raw ptr thats being passed int 
    explicit smart_ptr(T* raw_ptr); // DONE

    // copy constrctor 
    // put rhs data into this, check for different cases 
    smart_ptr(const smart_ptr& rhs); // DONE

    // move copy constructor, steal data from rhs and put into this
    // make rhs be empty 
    smart_ptr(smart_ptr&& rhs) noexcept;  // DONE

    // assighment operator, delete this's data and put in rhs's data 
    // check conditions and update the ref accordingly 
    smart_ptr& operator=(const smart_ptr& rhs); // DONE

    // move assighnment operator
    // 1st delete this's data 
    // 2nd put rhs's data into this
    // check for cases and then move
    // make rhs data point to nothing 
    smart_ptr& operator=(smart_ptr&& rhs) noexcept; // DONE

    // steal one ptr and make a deep copy of it 
    bool clone(); // DONE

    // return number of ref_counts to a smart pointer 
    // check for 1st condition of nullptr 
    /// else return the *ref_
    int ref_count() const; // DONE

    // using * return the value 
    T& operator*(); // DONE

    // using -> return the address 
    T* operator->(); // DONE

    // decrement ref_ and delte p_
    // delete noth at end 
    ~smart_ptr(); // DONE
};
#endif

template <typename T> 
T* smart_ptr<T> ::operator->() {
    if (this->ptr_ == nullptr)
        throw null_ptr_exception("error");
    return ptr_; 
}

template <typename T>
T& smart_ptr<T>::operator*() {
    //cout << "\n************ INSIDE * OPERATOR OVERLOAD **************\n"; 
    if (this->ptr_ == nullptr)
        throw null_ptr_exception("error");
    return *ptr_; 
}

template <typename T> 
bool smart_ptr<T> ::clone() {
    try {
        if (ptr_ == nullptr || *ref_ == 1)
            return false;
        if (ref_ != nullptr) {
            *ref_ = *ref_ - 1;
            this->ptr_ = new T(*ptr_);
            this->ref_ = new int(1);
        }
    }
    catch (bad_alloc) {
        cout << "\nNot enough space - bad alloc caught" << endl;
    }
    return true; 
}
template <typename T>
smart_ptr<T>& smart_ptr<T>::operator=(smart_ptr&& rhs) noexcept {
    // v2 = move(v1); 
   // cout << "\n******** MOVED ASSIGHNMENT OEPRATOR *************\n"; 
    if (this == &rhs)
        return *this; 
    if (ref_ != nullptr) {
        if (*ref_ > 1) {
            --(*ref_);
        }
        else {
            delete ptr_;
            delete ref_;
        }
    }
    this->ref_ = rhs.ref_; 
    this->ptr_ = rhs.ptr_; 
    rhs.ptr_ = nullptr; 
    rhs.ref_ = nullptr; 
    return *this; 
}

template <typename T> 
int smart_ptr<T>::ref_count() const {
    if (this->ref_ == nullptr)
        return 0; 
    return *ref_; 
}
template<typename T>
smart_ptr<T>::~smart_ptr() {
    if (ref_ != nullptr) {
        if (*ref_ > 1) {
            --(*ref_);
        }
        else {
            delete ptr_;
            delete ref_;
        }
    }
}

template<typename T>
smart_ptr<T>& smart_ptr<T>::operator=(const smart_ptr& rhs) {
   // cout << "\n********** IN ASSIGHNMENT OPERATOR ************\n";
    // sp2 = sp1; [both cases]
    if (this == &rhs)
        return *this;
    if (this->ref_ == nullptr) {
        this->ptr_ = rhs.ptr_; 
        this->ref_ = rhs.ref_; 
        if (this->ref_ != nullptr)
            *ref_ = *ref_ + 1;
    }
    else if (*ref_ >= 2) {
        *ref_ = *ref_ - 1;
        this->ptr_ = rhs.ptr_;
        this->ref_ = rhs.ref_;
        if (this->ref_ != nullptr)
            *ref_ = *ref_ + 1;
    }
    else if (*ref_ == 1) {
        delete this->ref_;
        delete this->ptr_;
        this->ptr_ = rhs.ptr_;
        this->ref_ = rhs.ref_;
        if (this->ref_ != nullptr)
            *ref_ = *ref_ + 1;
    }
    return *this;
}

template <typename T> 
smart_ptr<T> ::smart_ptr(smart_ptr&& rhs) noexcept {
    // smart_ptr sp2 = (move(sp1));
    //cout << "\n********** INSIDE MOVE COPY CONSTRUCTOR ************\n";
    this->ptr_ = rhs.ptr_; 
    this->ref_ = rhs.ref_; 
    rhs.ptr_ = nullptr; 
    rhs.ref_ = nullptr; 
}
template <typename T> 
smart_ptr<T> ::smart_ptr(const smart_ptr& rhs) {
   // cout << "\n********** INSIDE REULAR COPY CONSTRUCTOR ************\n"; 
    this->ptr_ = rhs.ptr_; 
    this->ref_ = rhs.ref_;
    if (this->ref_ != nullptr)
        *ref_ = *ref_ + 1;
}

template <typename T> 
void smart_ptr <T> ::print(string msg ) const {
    cout << "\n********* " << msg << " ************\n";
    if (this->ptr_ == nullptr ) {
        cout << "POINTING TO NOTHIG" << endl; 
        cout << "ptr_ = " << this->ptr_
            << ", ref_ = " << this->ref_ << endl; 
    }
    else {
        cout << "ptr_ Address = " << this->ptr_
            <<", ptr_ value = "<<*ptr_
            << ", ref_ = " << *ref_ << endl;
    }
    return;
}
template <typename T> 
smart_ptr<T> ::smart_ptr(T* raw_ptr) {
  //  cout << "\n********* IN PARAMETERIZED CONSTRUCTOR ************\n";
    // smart_ptr sp2(rawPtr); 
    try {
        this->ptr_ = raw_ptr;
        if (raw_ptr != nullptr)
            this->ref_ = new int(1);
        else
            this->ref_ = nullptr;
    }
    catch (bad_alloc) {
        cout << "\nNot enough space - bad alloc caught" << endl;
    }
}

template <typename T>
smart_ptr<T> ::smart_ptr() {
  //  cout << "\n********* IN DEFUALT CONSTRUCTOR ************\n"; 
    this->ptr_ = nullptr; 
    this->ref_ = nullptr; 
}