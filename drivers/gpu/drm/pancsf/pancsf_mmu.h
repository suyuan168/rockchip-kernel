/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright 2019 Linaro, Ltd, Rob Herring <robh@kernel.org> */
/* Copyright 2023 Collabora ltd. */

#ifndef __PANCSF_MMU_H__
#define __PANCSF_MMU_H__

struct pancsf_gem_object;
struct pancsf_vm;
struct pancsf_vma;
struct pancsf_mmu;
struct pancsf_vm_bind_job;

int pancsf_vm_remap_mcu_pages(struct pancsf_vm *vm,
			      struct drm_mm_node *mm_node,
			      struct sg_table *sgt,
			      int prot);
int pancsf_vm_map_mcu_pages(struct pancsf_vm *vm,
			    struct drm_mm_node *mm_node,
			    struct sg_table *sgt,
			    unsigned int num_pages,
			    u64 va_start, u64 va_end,
			    int prot);
void pancsf_vm_unmap_mcu_pages(struct pancsf_vm *vm,
			       struct drm_mm_node *mm_node);

int pancsf_mmu_init(struct pancsf_device *pfdev);
void pancsf_mmu_fini(struct pancsf_device *pfdev);
void pancsf_mmu_pre_reset(struct pancsf_device *pfdev);
void pancsf_mmu_reset(struct pancsf_device *pfdev);

int pancsf_vm_map_bo_range(struct pancsf_vm *vm, struct pancsf_gem_object *bo,
			   u64 offset, size_t size, u64 *va, u32 flags);
int pancsf_vm_unmap_range(struct pancsf_vm *vm, u64 va, size_t size);
struct pancsf_gem_object *
pancsf_vm_get_bo_for_vma(struct pancsf_vm *vm, u64 va, u64 *bo_offset);

int pancsf_vm_as_get(struct pancsf_vm *vm);
void pancsf_vm_as_put(struct pancsf_vm *vm);

struct pancsf_vm *pancsf_vm_get(struct pancsf_vm *vm);
void pancsf_vm_put(struct pancsf_vm *vm);
struct pancsf_vm *pancsf_vm_create(struct pancsf_device *pfdev, bool for_mcu);

struct dma_resv *pancsf_vm_resv(struct pancsf_vm *vm);

void pancsf_vm_pool_destroy(struct pancsf_file *pfile);
int pancsf_vm_pool_create(struct pancsf_file *pfile);
int pancsf_vm_pool_create_vm(struct pancsf_device *pfdev, struct pancsf_vm_pool *pool);
void pancsf_vm_pool_destroy_vm(struct pancsf_vm_pool *pool, u32 handle);
struct pancsf_vm *pancsf_vm_pool_get_vm(struct pancsf_vm_pool *pool, u32 handle);

void pancsf_vm_bind_queue_pool_destroy(struct pancsf_file *pfile);
int pancsf_vm_bind_queue_pool_create(struct pancsf_file *pfile);
int pancsf_vm_bind_queue_pool_create_queue(struct pancsf_device *pfdev,
					   struct pancsf_vm_bind_queue_pool *pool,
					   u8 prio);
void pancsf_vm_bind_queue_pool_destroy_queue(struct pancsf_vm_bind_queue_pool *pool,
					     u32 handle);
struct pancsf_vm_bind_queue *
pancsf_vm_bind_queue_pool_get_queue(struct pancsf_vm_bind_queue_pool *pool, u32 handle);

struct pancsf_vm_bind_queue *pancsf_vm_bind_queue_get(struct pancsf_vm_bind_queue *queue);
void pancsf_vm_bind_queue_put(struct pancsf_vm_bind_queue *queue);

void pancsf_vm_bind_job_destroy(struct pancsf_vm_bind_job *job);
struct pancsf_vm_bind_job *
pancsf_vm_bind_job_create(struct drm_file *file,
			  struct pancsf_vm_bind_queue *queue,
                          struct pancsf_vm *vm,
                          struct drm_pancsf_vm_bind_op *bind_ops,
                          u32 bind_op_count);
int pancsf_vm_bind_job_add_dep(struct pancsf_vm_bind_job *job, struct dma_fence *dep);
int pancsf_vm_bind_job_push(struct pancsf_vm_bind_job *job);
struct dma_fence *pancsf_vm_bind_job_done_fence(struct pancsf_vm_bind_job *job);

#endif
