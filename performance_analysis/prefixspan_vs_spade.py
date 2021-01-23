import analyzer

if __name__ == "__main__":
    sequence_miner_path = '../build/app/miner'

    # files = ['synthetic10k.spmf', 'synthetic20k.spmf', 'synthetic50k.spmf']
    files = ['fifa.spmf']
    files_directory = '../data/'
    min_support = [0.15]
    algorithms = ['PrefixSpan', 'SPADE']
    # algorithms = ['PrefixSpan']

    results_file = './prefixspan_vs_spade_fifa.csv'
    # results_file = './temp.csv'

    # with open(results_file, 'w') as fw:
        # fw.write('algorithm,input_file,all_sequences,min_support,frequenct_sequences,time,real_time\n')

    for f in files  :
        file_path = files_directory + f
        lines = analyzer.count_lines(file_path)
        for sup in min_support:
            for alg in algorithms:
                out_ps = 'out_{}_{}_{}.txt'.format(f[:-5], sup, alg)
                ps = analyzer.benchmark(sequence_miner_path, file_path, alg, sup, out_ps, True)

                with open(results_file, 'a') as fa:
                    fa.write('{},{},{},{},{},{},{}\n'.format(alg,f,lines,sup,ps['sequences_count'],ps['time'], ps['real_time']))
