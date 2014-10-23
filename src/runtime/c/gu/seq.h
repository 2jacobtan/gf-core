#ifndef GU_SEQ_H_
#define GU_SEQ_H_

#include <gu/mem.h>

typedef struct {
	size_t len;
	uint8_t data[0];
} GuSeq;

typedef struct {
	GuSeq* seq;
	size_t elem_size;
	size_t avail_len;
	GuFinalizer fin;
} GuBuf;

GuSeq*
gu_empty_seq();

GuSeq*
gu_make_seq(size_t elem_size, size_t len, GuPool* pool);

#define gu_new_seq(T, N, POOL)			\
	gu_make_seq(sizeof(T), (N), (POOL))

GuSeq*
gu_alloc_seq_(size_t elem_size, size_t length);

#define gu_alloc_seq(T, N)			\
	gu_alloc_seq_(sizeof(T), (N))

GuSeq*
gu_realloc_seq_(GuSeq* seq, size_t elem_size, size_t length);

#define gu_realloc_seq(S, T, N)			\
	gu_realloc_seq_(S, sizeof(T), (N))

void
gu_seq_free(GuSeq* seq);

inline size_t
gu_seq_length(GuSeq* seq)
{
	return seq->len;
}

inline void*
gu_seq_data(GuSeq* seq)
{
	return seq->data;
}

#define gu_seq_index(SEQ, T, I)			\
	(&((T*)gu_seq_data(SEQ))[I])

#define gu_seq_get(SEQ, T, I)			\
	(*gu_seq_index(SEQ, T, I))

#define gu_seq_set(SEQ, T, I, V)			\
	GU_BEGIN				\
	(*gu_seq_index(SEQ, T, I) = (V));	\
	GU_END


GuBuf*
gu_make_buf(size_t elem_size, GuPool* pool);

#define gu_new_buf(T, POOL)			\
	gu_make_buf(sizeof(T), (POOL))

inline size_t
gu_buf_length(GuBuf* buf)
{
	return buf->seq->len;
}

inline size_t
gu_buf_avail(GuBuf* buf)
{
	return buf->avail_len;
}

inline void*
gu_buf_data(GuBuf* buf)
{
	return &buf->seq->data;
}

inline GuSeq*
gu_buf_data_seq(GuBuf* buf)
{
	return buf->seq;
}

#define gu_buf_index(BUF, T, I)			\
	(&((T*)gu_buf_data(BUF))[I])

#define gu_buf_get(BUF, T, I)			\
	(*gu_buf_index(BUF, T, I))

#define gu_buf_set(BUF, T, I, V)		\
	GU_BEGIN				\
	(*gu_buf_index(BUF, T, I) = (V));	\
	GU_END

void
gu_buf_push_n(GuBuf* buf, const void* elems, size_t n_elems);

void*
gu_buf_extend_n(GuBuf* buf, size_t n_elems);

inline void*
gu_buf_extend(GuBuf* buf)
{
	return gu_buf_extend_n(buf, 1);
}

#define gu_buf_push(BUF, T, VAL)				\
	GU_BEGIN						\
	((*(T*)gu_buf_extend(BUF)) = (VAL));	\
	GU_END

void
gu_buf_pop_n(GuBuf* buf, size_t n_elems, void* data_out);

const void*
gu_buf_trim_n(GuBuf* buf, size_t n_elems);

inline const void*
gu_buf_trim(GuBuf* buf)
{
	return gu_buf_trim_n(buf, 1);
}

void*
gu_buf_insert(GuBuf* buf, size_t n_index);

inline void
gu_buf_flush(GuBuf* buf)
{
	buf->seq->len = 0;
}

#define gu_buf_pop(BUF, T)			\
	(*(T*)gu_buf_trim(BUF))

void
gu_buf_sort(GuBuf *buf, GuOrder *order);

#define gu_seq_binsearch(S, O, T, V) \
	((T*) gu_seq_binsearch_(S, O, sizeof(T), V))

void*
gu_seq_binsearch_(GuSeq *seq, GuOrder *order, size_t elem_size, const void *key);

#define gu_seq_binsearch_index(S, O, T, V, PI) \
	gu_seq_binsearch_index_(S, O, sizeof(T), V, PI)

bool
gu_seq_binsearch_index_(GuSeq *seq, GuOrder *order, size_t elem_size, 
                        const void *key, size_t *pindex);

// Using a buffer as a heap
void
gu_buf_heap_push(GuBuf *buf, GuOrder *order, void *value);

void
gu_buf_heap_pop(GuBuf *buf, GuOrder *order, void* data_out);

void
gu_buf_heap_replace(GuBuf *buf, GuOrder *order, void *value, void *data_out);

void
gu_buf_heapify(GuBuf *buf, GuOrder *order);

GuSeq*
gu_buf_freeze(GuBuf* buf, GuPool* pool);
#endif // GU_SEQ_H_

#if defined(GU_OUT_H_) && !defined(GU_SEQ_H_OUT_)
#define GU_SEQ_H_OUT_

GuOut*
gu_buf_out(GuBuf* buf, GuPool* pool);

#endif


#if defined(GU_TYPE_H_) && !defined(GU_SEQ_H_TYPE_)
#define GU_SEQ_H_TYPE_

extern GU_DECLARE_KIND(GuSeq);
extern GU_DECLARE_KIND(GuBuf);

struct GuSeqType {
	GuType_GuOpaque opaque_base;
	GuType* elem_type;
};

typedef const struct GuSeqType GuSeqType, GuType_GuSeq;

#define GU_TYPE_INIT_GuSeq(k_, t_, elem_type_) {	   \
	.opaque_base = GU_TYPE_INIT_GuOpaque(k_, t_, _), \
	.elem_type = elem_type_,		   	\
}

typedef struct GuBufType GuBufType, GuType_GuBuf;

struct GuBufType {
	GuType_abstract abstract_base;
	GuType* elem_type;
};

#define GU_TYPE_INIT_GuBuf(KIND, BUF_T, ELEM_T) { \
	.abstract_base = GU_TYPE_INIT_abstract(KIND, BUF_T, _), \
	.elem_type = ELEM_T \
}

#endif 

