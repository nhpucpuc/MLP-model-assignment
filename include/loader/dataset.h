/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/* 
 * File:   dataset.h
 * Author: ltsach
 *
 * Created on September 2, 2024, 3:59 PM
 */

#ifndef DATASET_H
#define DATASET_H
#include "tensor/xtensor_lib.h"
using namespace std;

template<typename DType, typename LType>
class DataLabel{
private:
    xt::xarray<DType> data;
    xt::xarray<LType> label;
public:
    DataLabel(xt::xarray<DType> data,  xt::xarray<LType> label):
    data(data), label(label){
    }
    xt::xarray<DType> getData() const{ return data; }
    xt::xarray<LType> getLabel() const{ return label; }
};

template<typename DType, typename LType>
class Batch{
private:
    xt::xarray<DType> data;
    xt::xarray<LType> label;
public:
    Batch(xt::xarray<DType> data,  xt::xarray<LType> label):
    data(data), label(label){
    }
    virtual ~Batch(){}
    xt::xarray<DType>& getData(){return data; }
    xt::xarray<LType>& getLabel(){return label; }
};


template<typename DType, typename LType>
class Dataset{
private:
public:
    Dataset(){};
    virtual ~Dataset(){};
    
    virtual int len()=0;
    virtual DataLabel<DType, LType> getitem(int index)=0;
    virtual xt::svector<unsigned long> get_data_shape()=0;
    virtual xt::svector<unsigned long> get_label_shape()=0;
    
};

//////////////////////////////////////////////////////////////////////
template<typename DType, typename LType>
class TensorDataset: public Dataset<DType, LType>{
private:
    xt::xarray<DType> data;
    xt::xarray<LType> label;
    xt::svector<unsigned long> data_shape, label_shape;
    
public:
    /* TensorDataset: 
     * need to initialize:
     * 1. data, label;
     * 2. data_shape, label_shape
    */
    TensorDataset(xt::xarray<DType> data, xt::xarray<LType> label)
    {
        // TODO implement
        this->data = data;
        this->data_shape = data.shape();
        this->label_shape = label.shape();
        this->label = label;
    }
    /* len():
     *  return the size of dimension 0
    */
    int len() 
    {
        // TODO implement
        return this->data_shape[0];
    }
    
    /* getitem:
     * return the data item (of type: DataLabel) that is specified by index
     */
    DataLabel<DType, LType> getitem(int index) 
    {
        // TODO implement
        if(index < 0 || index >= this->len())
        {
        throw std::out_of_range("Index is out of range!");
        }
        if(index >= 0 && index < this->len())
        {
        xt::xarray<DType> temp_data;
        xt::xarray<LType> temp_label;
        if(this->label_shape.size() == 0 )
        {
            xt::xarray<DType> temp_data = xt::view(this->data, index, xt::all());
            xt::xarray<LType> temp_label = xt::xarray<LType>({this->label});   
            return DataLabel<DType, LType>(temp_data, temp_label);      
        }else
        {
            xt::xarray<DType> temp_data = xt::view(this->data, index, xt::all());
            xt::xarray<LType> temp_label = xt::view(this->label, index, xt::all());
            return DataLabel<DType, LType>(temp_data, temp_label);
        }
        }else{throw std::out_of_range("Index is out of range!");}
    }
    
    xt::svector<unsigned long> get_data_shape() { return data_shape; }

    xt::svector<unsigned long> get_label_shape() { return label_shape; }
};



#endif /* DATASET_H */

