/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/* 
 * File:   dataloader.h
 * Author: ltsach
 *
 * Created on September 2, 2024, 4:01 PM
 */

#ifndef DATALOADER_H
#define DATALOADER_H
#include "tensor/xtensor_lib.h"
#include "loader/dataset.h"

using namespace std;

template<typename DType, typename LType>
class DataLoader{
public:
    class Iterator; //forward declaration for class Iterator
    
private:
    Dataset<DType, LType>* ptr_dataset;
    int batch_size;
    bool shuffle;
    bool drop_last;
    int nbatch;
    ulong_tensor item_indices;
    int m_seed;
    
public:
    DataLoader(Dataset<DType, LType>* ptr_dataset, 
            int batch_size, bool shuffle=true, 
            bool drop_last=false, int seed=-1)
                : ptr_dataset(ptr_dataset), 
                batch_size(batch_size), 
                shuffle(shuffle),
                m_seed(seed){
            nbatch = ptr_dataset->len()/batch_size;
            item_indices = xt::arange(0, ptr_dataset->len());
            shuffle_index();
    }
    void shuffle_index()
    {
        if(shuffle == 1)
        {
        if(this->m_seed >= 0)
        {
            xt::random::seed(this->m_seed);
        }
        xt::random::shuffle(item_indices);
        }
    }
    virtual ~DataLoader(){}
    
    //New method: from V2: begin
    int get_batch_size(){ return batch_size; }
    int get_sample_count(){ return ptr_dataset->len(); }
    int get_total_batch(){return int(ptr_dataset->len()/batch_size); }
    
    //New method: from V2: end
    /////////////////////////////////////////////////////////////////////////
    // The section for supporting the iteration and for-each to DataLoader //
    /// START: Section                                                     //
    /////////////////////////////////////////////////////////////////////////
public:
    Iterator begin() 
    {
        return Iterator(this, 0);
    }

    Iterator end() 
    {
        return Iterator(this, ptr_dataset->len()/batch_size);
    }
    
    class Iterator 
    {
    private:
        int nowBatch;
        DataLoader<DType, LType> *dataLoader;
    public:
        Iterator(DataLoader<DType, LType> *dataLoader, int nowBatch)
        {
        this->nowBatch = nowBatch;  
        this->dataLoader = dataLoader;
        }
        Iterator& operator=(const Iterator& iterator) 
        {
        if(this == &iterator)
        {
            return *this;
        }
        this->dataLoader = iterator.dataloader;
        this->nowBatch = iterator.nowBatch;
        }

        Iterator& operator++() 
        {
        nowBatch += 1;
        return *this;
        }

        Iterator operator++(int) 
        {
        Iterator This = *this;
        ++(*this);
        return This;
        }

        bool operator!=(const Iterator& other) const 
        {
        return !(this->nowBatch == other.nowBatch);
        }

        Batch<DType, LType> operator*() const 
        {
        int dataSize = dataLoader->ptr_dataset->len();
        int startBatch = nowBatch * dataLoader->batch_size;
        int endBatch = 0;
        int indexOfdata = 0;
        if(dataLoader->drop_last == 0)
        {
            if((nowBatch == (dataSize / dataLoader->batch_size - 1)) || dataSize/dataLoader->batch_size <= 1)
            {
            endBatch = dataSize;
            }else
            {
            endBatch = startBatch + dataLoader->batch_size;
            }
        }else
        {
            endBatch = startBatch + dataLoader->batch_size;
        }

        xt::xarray<DType> dataOfBatch;
        xt::xarray<LType> lableOfBatch;
        size_t sizeOfShape = dataLoader->ptr_dataset->get_label_shape().size();
        if(sizeOfShape != 0)
        {
            xt::svector<unsigned long> shapeOflabel = dataLoader->ptr_dataset->get_label_shape();
            shapeOflabel[0] = static_cast<unsigned long>(endBatch - startBatch);
            lableOfBatch = xt::empty<LType>(shapeOflabel);

            xt::svector<unsigned long> shapeOfdata = dataLoader->ptr_dataset->get_data_shape();
            shapeOfdata[0] = static_cast<unsigned long>(endBatch - startBatch);
            dataOfBatch = xt::empty<DType>(shapeOfdata);        
        }else
        {
            xt::svector<unsigned long> shapeOfdata = dataLoader->ptr_dataset->get_data_shape();
            shapeOfdata[0] = static_cast<unsigned long>(endBatch - startBatch);
            dataOfBatch = xt::empty<DType>(shapeOfdata);        
        }

        int i = startBatch; 
        while(i < endBatch) 
        {
            if(sizeOfShape != 0)
            {
                xt::view(lableOfBatch, indexOfdata, xt::all()) = dataLoader->ptr_dataset->getitem(this->dataLoader->item_indices[i]).getLabel();
            }
            xt::view(dataOfBatch, indexOfdata, xt::all()) = dataLoader->ptr_dataset->getitem(this->dataLoader->item_indices[i]).getData();
            
            indexOfdata++;  
            i++;            
        }
            return Batch<DType, LType>(dataOfBatch, lableOfBatch);
        }
    };
    //BEGIN of Iterator

    //YOUR CODE IS HERE: to define iterator

    //END of Iterator
    
    /////////////////////////////////////////////////////////////////////////
    // The section for supporting the iteration and for-each to DataLoader //
    /// END: Section                                                       //
    /////////////////////////////////////////////////////////////////////////
};


#endif /* DATALOADER_H */

