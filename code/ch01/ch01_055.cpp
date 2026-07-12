const Instruction& findInst(const std::vector<Instruction>& list, int id) {
       for (const auto& inst : list)
           if (inst.id() == id) return inst;
       // Oops -- what do we return here?
   }
