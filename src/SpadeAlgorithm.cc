#include "SpadeAlgorithm.h"

#include <algorithm>
#include <chrono>
#include <execution>

#include "SequenceData.h"

void SpadeAlgorithm::setup(int minSupport, bool dfs) {
  min_support_ = minSupport;
  dfs_ = dfs;
}

bool SpadeAlgorithm::run(int minSupport) {
  std::cout << "Run SPADE algorithm" << std::endl;

  min_support_ = minSupport;

  std::chrono::steady_clock::time_point begin =
      std::chrono::steady_clock::now();

  const auto root = std::make_shared<EquivalenceClass>();

  std::cout << "Finding frequent 1-sequences...\t" << std::flush;
  auto frequentSingleItems = input_.getSingleFrequentItemClasses(min_support_);
  std::cout << "✓\n";
  std::cout << "Finding frequent 2-sequences...\t" << std::flush;
  const auto frequentDoubleItems = input_.getDoubleFrequentItemClasses(min_support_);
  std::cout << "✓\n" << std::flush;
  for (const auto& seq2 : frequentDoubleItems) {
    insertClassByPrefix(seq2, frequentSingleItems);
  }
  root->setMembers(frequentSingleItems);

  const auto members = root->getMembers();
  pushToFinalSequences(members);

  std::cout << "Enumerating remaining frequent sequences...\t" << std::flush;
  std::for_each(std::execution::par, members.begin(), members.end(),
                [&](const auto& eq) { enumerateFrequentSequences(eq); });
  std::cout << "✓\n" << std::flush;

  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  std::cout << "Execution time = "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                     begin)
                   .count()
            << " [ms]" << std::endl;

  std::cout << "Found " << final_sequences_.size() << " frequent sequences"
            << std::endl;

  return true;
}

void SpadeAlgorithm::enumerateFrequentSequences(const EquivalenceClass_& eq) {
  auto atoms = eq->getMembers();
  bool anyFrequentFound = false;

  for (int i = 0; i < atoms.size(); ++i) {
    for (int j = i; j < atoms.size(); ++j) {
      for (const auto& candidate :
           generateJoinedCandidates(atoms[i], atoms[j])) {

        if (candidate->support() > min_support_) {
          anyFrequentFound = true;
          insertClassByPrefix(candidate, atoms[i], atoms[j]);
        }
      }
    }
    if (dfs_ && anyFrequentFound) {
      enumerateFrequentSequences(atoms[i]);
    }
  }

  pushToFinalSequences(atoms);

  if (!dfs_ && anyFrequentFound) {
    const auto members = eq->getMembers();

    std::for_each(std::execution::par, members.begin(), members.end(),
                  [&](const auto& eq) { enumerateFrequentSequences(eq); });
  }
}

void SpadeAlgorithm::insertClassByPrefix(
    const EquivalenceClass_& eq, std::vector<EquivalenceClass_>& parents) {
  for (auto& p : parents) {
    if (p->isParentOf(eq)) {
      p->addMember(eq);
      return;
    }
  }
  // FIXME other error maybe
  throw std::out_of_range("No parent with matching prefix");
}

void SpadeAlgorithm::insertClassByPrefix(const EquivalenceClass_& eq,
                                         EquivalenceClass_& parent1,
                                         EquivalenceClass_& parent2) {
  if (parent1->isParentOf(eq)) {
    parent1->addMember(eq);
  } else if (parent2->isParentOf(eq)) {
    parent2->addMember(eq);
  } else {
    // FIXME other error maybe
    throw std::out_of_range("No parent with matching prefix");
  }
}

std::vector<EquivalenceClass_> SpadeAlgorithm::generateCandidates(
    const EquivalenceClass_& eq1, const EquivalenceClass_& eq2) {
  const auto presuf1 = eq1->getPrefixSuffixSeqParts();
  const auto presuf2 = eq2->getPrefixSuffixSeqParts();

  if (presuf1.first != presuf2.first) {
    throw std::invalid_argument(
        "Provided classes are not of the same class - don't have the same "
        "prefix.");
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

std::vector<EquivalenceClass_> SpadeAlgorithm::generateJoinedCandidates(
    const EquivalenceClass_& eq1, const EquivalenceClass_& eq2) {
  auto candidates = generateCandidates(eq1, eq2);
  for (auto& candidate : candidates) {
    candidate->joinIdList(eq1, eq2);
  }
  return candidates;
}

void SpadeAlgorithm::pushToFinalSequences(
    const std::vector<EquivalenceClass_>& eqClasses) {
  std::lock_guard<std::mutex> lock(final_sequences_mutex_);
  for (const auto& cl : eqClasses) {
    final_sequences_.push_back(cl->getSequence());
  }
}
