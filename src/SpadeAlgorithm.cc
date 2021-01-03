#include "SpadeAlgorithm.h"

#include <algorithm>
#include <execution>
#include <chrono>

#include "SequenceData.h"

bool SpadeAlgorithm::loadData(SequenceData input) {
  std::cout << "Load data for SPADE algorithm" << std::endl;
  input_ =  std::move(input);
  return true;
}

bool SpadeAlgorithm::run(int min_support) { 
  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
  std::cout << "Run SPADE algorithm" << std::endl;
  min_support_ = min_support;
  // input_.removeInfrequentItems(min_support_);
  input_.printData();

  auto root = std::make_shared<EquivalenceClass>();

  auto frequentSingleItems = input_.getSingleFrequentItemClasses(min_support_);
  auto frequentDoubleItems = input_.getDoubleFrequentItemClasses(min_support_);
  for (const auto& seq2 : frequentDoubleItems) {
    insertClassByPrefix(seq2, frequentSingleItems);
  }
  root->setMembers(frequentSingleItems);

  // for (auto& eq : root.getMembers()) {
  //   eq.print();
  // }

  auto members = root->getMembers();
  pushToFinalSequences(members);

  std::for_each(
    std::execution::par,
    members.begin(),
    members.end(),
    [&] (auto& eq) {
      enumerateFrequentSequences(eq); 
    }
  );

  // root->setMembers(members);

  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  std::cout << "Execution time = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;

  printFinalSequences();
  return true;
}

void SpadeAlgorithm::enumerateFrequentSequences(EquivalenceClass_& eq) {
  std::cout << "Enumerating" << std::endl << std::endl;

  eq->print();
  auto atoms = eq->getMembers();
  bool anyFrequentFound = false;

  for (int i = 0; i < atoms.size(); ++i) {
    for (int j = i; j < atoms.size(); ++j) {
      // std::cout << "generating for " << i << " " << j << std::endl;
      for (const auto& candidate : generateJoinedCandidates(atoms[i], atoms[j])) {
        candidate->print();
        if (candidate->support() > min_support_) {
          anyFrequentFound = true;
          insertClassByPrefix(candidate, atoms[i], atoms[j]);
        }
        // std::cout << "LAST PAIR" << std::endl;
        // for (auto el : eq.getLastSeqPair()) {
        //   std::cout << el << " ";
        // }
        // std::cout << std::endl;
      }
    }
  }
  // eq->setMembers(atoms);
  pushToFinalSequences(atoms);

  if (anyFrequentFound) {
    auto members = eq->getMembers();

    std::for_each(
      std::execution::par,
      members.begin(),
      members.end(),
      [&] (auto& eq) {
        enumerateFrequentSequences(eq); 
      }
    );
  }
}

void SpadeAlgorithm::insertClassByPrefix(const EquivalenceClass_& eq, std::vector<EquivalenceClass_>& parents) const {
  for (auto &p : parents) {
    if (p->isParentOf(eq)) {
      p->addMember(eq);
      return;
    }
  }
  // FIXME other error maybe
  throw std::out_of_range("No parent with matching prefix");
}

void SpadeAlgorithm::insertClassByPrefix(const EquivalenceClass_& eq, EquivalenceClass_& parent1, EquivalenceClass_& parent2) const {
  if (parent1->isParentOf(eq)) {
    parent1->addMember(eq);
  } else if (parent2->isParentOf(eq)) {
    parent2->addMember(eq);
  } else {
    // FIXME other error maybe
    throw std::out_of_range("No parent with matching prefix");
  }
}

std::vector<EquivalenceClass_> SpadeAlgorithm::generateCandidates(const EquivalenceClass_& eq1, const EquivalenceClass_& eq2) const {
  auto presuf1 = eq1->getPrefixSuffixSeqParts();
  auto presuf2 = eq2->getPrefixSuffixSeqParts();

  // std::cout << "prefix1 \t";
  // for (const auto& el : presuf1.first) {
  //   std::cout << el << " ";
  // }
  // std::cout << std::endl << "suffix1\t";
  //   for (const auto& el : presuf1.second) {
  //   std::cout << el << " ";
  // }
  // std::cout << std::endl;

  //   std::cout << "prefix2 \t";
  // for (const auto& el : presuf2.first) {
  //   std::cout << el << " ";
  // }
  // std::cout << std::endl << "suffix2\t";
  //   for (const auto& el : presuf2.second) {
  //   std::cout << el << " ";
  // }
  // std::cout << std::endl;

  if (presuf1.first != presuf2.first) {
    throw std::invalid_argument("Provided classes are not of the same class - don't have the same prefix.");
  }

  std::vector<EquivalenceClass_> candidates{};

  Sequence seq = presuf1.first;


  // atom + atom
  if (presuf1.second.size() == 1 && presuf2.second.size() == 1) {
    if (presuf1.second == presuf2.second) {
      return candidates;
    }
    seq.push_back(presuf1.second[0]);
    seq.push_back(presuf2.second[0]);
    candidates.push_back(std::make_shared<EquivalenceClass>(seq));
  }
  // sequence + sequence
  else if (presuf1.second.size() == 2 && presuf2.second.size() == 2) {
    // P -> S -> S
    if (presuf1.second == presuf2.second) {
      seq.insert(seq.end(), presuf1.second.begin(), presuf1.second.end());
      seq.insert(seq.end(), presuf2.second.begin(), presuf2.second.end());
      candidates.push_back(std::make_shared<EquivalenceClass>(seq));
      return candidates;
    }
    // P -> S1S2
    seq.insert(seq.end(), presuf1.second.begin(), presuf1.second.end());
    seq.push_back(presuf2.second.back());
    candidates.push_back(std::make_shared<EquivalenceClass>(seq));

    seq = presuf1.first;
    // P -> S1 -> S2
    seq.insert(seq.end(), presuf1.second.begin(), presuf1.second.end());
    seq.insert(seq.end(), presuf2.second.begin(), presuf2.second.end());
    candidates.push_back(std::make_shared<EquivalenceClass>(seq));

    seq = presuf1.first;
    // P -> S2 -> S1
    seq.insert(seq.end(), presuf2.second.begin(), presuf2.second.end());
    seq.insert(seq.end(), presuf1.second.begin(), presuf1.second.end());
    candidates.push_back(std::make_shared<EquivalenceClass>(seq));
  }
  // atom + sequence
  else if (presuf1.second.size() == 1) {
    seq.push_back(presuf1.second[0]);
    seq.insert(seq.end(), presuf2.second.begin(), presuf2.second.end());
    candidates.push_back(std::make_shared<EquivalenceClass>(seq));
  }
  // sequence + atom
  else {
    seq.push_back(presuf2.second[0]);
    seq.insert(seq.end(), presuf1.second.begin(), presuf1.second.end());
    candidates.push_back(std::make_shared<EquivalenceClass>(seq));
  }
  return candidates;
}

std::vector<EquivalenceClass_> SpadeAlgorithm::generateJoinedCandidates(const EquivalenceClass_& eq1, const EquivalenceClass_& eq2) const {
  auto candidates = generateCandidates(eq1, eq2);
  for (auto& candidate : candidates) {
    candidate->joinIdList(eq1, eq2);
  }
  return candidates;
}

void SpadeAlgorithm::pushToFinalSequences(const std::vector<EquivalenceClass_>& eqClasses) {
  for (const auto& cl : eqClasses) {
    final_sequences_.push_back(cl->getSequence());
  }
}