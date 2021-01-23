import analyzer

if __name__ == "__main__":
    sequence_miner_path = '../build/app/miner'
    spmf_path = 'spmf.jar'

    # files = ['synthetic10k.spmf', 'synthetic20k.spmf', 'synthetic50k.spmf']
    files = ['fifa.spmf']
    files_directory = '../data/'
    min_support = [0.3, 0.25, 0.2, 0.15]
    algorithms = ['PrefixSpan', 'SPADE']
    programs = [sequence_miner_path, spmf_path]
    # algorithms = ['PrefixSpan']

    results_file = './sm_vs_spmf_fifa.csv'

    with open(results_file, 'w') as fw:
        fw.write('program,algorithm,input_file,all_sequences,min_support,frequenct_sequences,time,real_time\n')

    for f in files  :
        file_path = files_directory + f
        lines = analyzer.count_lines(file_path)
        for sup in min_support:
            for alg in algorithms:
                for p in programs:
                    if p.endswith('miner'):
                        pname = 'miner'
                    else:
                        pname = 'spmf'

                    print('Running {} {} for file {} with support {}'.format(pname,alg, f, sup))
                    out_ps = 'out_{}_{}_{}_{}.txt'.format(pname,f[:-5], sup, alg)
                    ps = analyzer.benchmark(sequence_miner_path, file_path, alg, sup, out_ps, True)

                    with open(results_file, 'a') as fa:
                        fa.write('{},{},{},{},{},{},{},{}\n'.format(pname,alg,f,lines,sup,ps['sequences_count'],ps['time'], ps['real_time']))
