import analyzer

if __name__ == "__main__":
    sequence_miner_path = '../build/app/miner'

    # files = ['synthetic10k.spmf', 'synthetic20k.spmf', 'synthetic50k.spmf']
    files = ['bible.spmf']
    files_directory = '../data/'
    min_support = [0.1]

    results_file = './spade_dfs_bfs_bible_0_1.csv'
    # results_file = './temp.csv'

    with open(results_file, 'w') as fw:
        fw.write('algorithm,input_file,all_sequences,min_support,frequenct_sequences,time,real_time\n')

    for f in files  :
        file_path = files_directory + f
        lines = analyzer.count_lines(file_path)
        for sup in min_support:


            alg = 'SPADE_bfs'
            print('Running {} for file {} with support {}'.format(alg, f, sup))
            out_ps = 'out_{}_{}_{}.txt'.format(f[:-5], sup, alg)
            ps = analyzer.benchmark(sequence_miner_path, file_path, 'SPADE', sup, out_ps, True, False)

            with open(results_file, 'a') as fa:
                fa.write('{},{},{},{},{},{},{}\n'.format(alg,f,lines,sup,ps['sequences_count'],ps['time'], ps['real_time']))
                
            alg = 'SPADE_dfs'
            print('Running {} for file {} with support {}'.format(alg, f, sup))
            out_ps = 'out_{}_{}_{}.txt'.format(f[:-5], sup, alg)
            ps = analyzer.benchmark(sequence_miner_path, file_path, 'SPADE', sup, out_ps, True, True)

            with open(results_file, 'a') as fa:
                fa.write('{},{},{},{},{},{},{}\n'.format(alg,f,lines,sup,ps['sequences_count'],ps['time'], ps['real_time']))

