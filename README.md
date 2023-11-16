# 15-618-Final-Project
# Creating the README content

readme_content = """
# 15-618 Final Project Proposal

## Title

Performance evaluation on parallel address translation lookup using hardware page table walker supporting Elastic Cuckoo Page Table 

## URL: 
https://github.com/PeterDing372/15-618-Final-Project.git

## Summary

We will implement a hardware page table walker with Chisel (Scala) for an open-source RISC-V Core and evaluate its performance with RTL exact software simulation. The evaluation includes a look-up latency comparison between the traditional Radix Page Table and Elastic Cuckoo Page Table in terms of memory access statistics and delay. 

## Background

For applications that span a large memory footprint, address translation takes place often and can sometimes take up a large amount of the overall execution time. This is due to the nature of traditional Radix Page Tables requiring sequential memory access to fetch the page table entry (PTE). At the same time, scaling TLB is difficult as large TLBs require longer latency and power. The Elastic Cuckoo Page Table proposal attempts to address the critical path of page table walks[1]. The idea is to re-format the multi-level structure of traditional page tables into a flat layout enabling parallel look-up to all ways of the page table entries.  

Figures 1, and 2 (Skarlatos et al., 2020) are adapted from the original paper for illustration. [Figure 1](https://www.notion.so/15-618-Final-Project-Proposal-e704374c13914f7a971ab86b94ce65d7?pvs=21) shows the traditional Radix page table layout which requires 4 sequential accesses to fetch the PTE. [Figure 2](https://www.notion.so/15-618-Final-Project-Proposal-e704374c13914f7a971ab86b94ce65d7?pvs=21) shows the ECPT layout being flat where the page table walker can issue parallel memory requests to fetch the PTEs. Each way is a hashed page table and each entry is fetched with the hashed VPN Tag to index into the page tables. 

![Figure 1: Radix page table address translation in x86-64. Adapted from Skarlatos, D., Kokolis, A., Xu, T., & Torrellas, J. (2020)](https://prod-files-secure.s3.us-west-2.amazonaws.com/f7b9fbbc-e9c9-4123-948b-2b5fa6d239d0/4ca33efe-3c2a-48cc-8e0e-6caab3640655/Untitled.png)

Figure 1: Radix page table address translation in x86-64. Adapted from Skarlatos, D., Kokolis, A., Xu, T., & Torrellas, J. (2020)

![Figure 2: Elastic cuckoo page tables for a process. Adapted from Skarlatos, D., Kokolis, A., Xu, T., & Torrellas, J. (2020)](https://prod-files-secure.s3.us-west-2.amazonaws.com/f7b9fbbc-e9c9-4123-948b-2b5fa6d239d0/5c748880-af5b-4111-b249-3b13b5bd5f6f/Untitled.png)

Figure 2: Elastic cuckoo page tables for a process. Adapted from Skarlatos, D., Kokolis, A., Xu, T., & Torrellas, J. (2020)

Building on the original paper’s evaluation methodology, we want a more comprehensive evaluation with a full-system evaluation where the Page Table Walker is fully embedded into a robust processor. We will use the Berkeley Out-of-Order Machine (BOOM) as our target processor[2]. To do RTL exact software emulation, we will use the Chipyard [3] to compile the processor. 

## The Challenge

- **Cache Line pollution:** For each address translation, the lookups to different ways in the page table do not have a dependency, however, the total number of requests made to the memory can potentially be larger than the traditional page table. This could potentially pollute our cache lines evicting cache entries that may be referenced in the near future.
- **Larger L1 Cache access:** Page table entry clustering allows 8 consecutive PTE entries to be put in the same 64-byte cache line and spread the tags to the reserved bits for each PTE. This means the number of accesses to L1 cache is increased as we have to fetch all 8 PTEs to make a tag match. In addition, as cache ports are 64-bits wide, we need to make 8 sequential accesses to the cache to get the hit result of one
