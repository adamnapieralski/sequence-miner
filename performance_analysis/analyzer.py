
import time
import subprocess


def sequence_miner_min_support(min_support, file):
    lines = count_lines(file)
    return int(min_support*lines) - 1

def count_lines(file):
    with open(file) as f:
        for i, _ in enumerate(f):
            pass
    return i+1

def run_spmf(spmf_path, input_file, algorithm, min_support, output_file):
    results = subprocess.run(['java', '-jar', spmf_path, 'run', algorithm, input_file, output_file, str(min_support)], capture_output=True)
    return results.stdout

def create_config(fname, algorithm, min_support, input_file, dfs, output_file, output_items_separator, output_events_separator):

    config = (
        "input_file: {input_file}\n"
        "input_ids_separator: {input_ids_separator}\n"
        "input_items_separator: {input_items_separator}\n"
        "min_support: {support}\n"
        "algorithm: {algorithm}\n"
        "spade_dfs: {dfs}\n"
        "data_type: 'int'\n"
        "output_file: {output_file}\n"
        "output_items_separator: {output_items_separator}\n"
        "output_events_separator: {output_events_separator}\n"

    )
    config = config.format(
        input_file=input_file,
        input_ids_separator='" "',
        input_items_separator='" "',
        support=min_support,
        algorithm=algorithm.lower(),
        dfs=str(dfs).lower(),
        output_file=output_file,
        output_items_separator='"' + output_items_separator + '"',
        output_events_separator='"' + output_events_separator + '"'
    )

    with open(fname, 'w') as f:
        f.write(config)

def run_sequence_miner(sequence_miner_path, input_file, algorithm, min_support, output_file, dfs=True):
    config = 'temp_config.yaml'
    create_config(config, algorithm, min_support, input_file, dfs, output_file, ' ', '->')

    results = subprocess.run([sequence_miner_path, config], capture_output=True)
    return results.stdout

def process_spmf_stdout(out, print_out=False):
    text = out.decode('utf-8')

    if print_out:
        print(text)

    lines = text.split('\n')
    results = dict()

    for l in lines:
        if 'Total time' in l:
            res = [int(i) for i in l.split(' ') if i.isdigit()]
            results['time'] = res[0]
        if 'Frequent sequences count' in l:
            res = [int(i) for i in l.split(' ') if i.isdigit()]
            results['sequences_count'] = res[0]

    return results

def process_sequence_miner_stdout(out, print_out=False):
    text = out.decode('utf-8')

    if print_out:
        print(text)

    lines = text.split('\n')
    results = dict()

    for l in lines:
        if 'Execution time' in l:
            res = [int(i) for i in l.split(' ') if i.isdigit()]
            results['time'] = res[0]
        if 'Found' in l and 'frequent sequences' in l:
            res = [int(i) for i in l.split(' ') if i.isdigit()]
            results['sequences_count'] = res[0]

    return results


def benchmark(program, input_file, algorithm, min_support, output_file, print_out=False):
    if program.endswith('spmf.jar'):
        s = time.time()
        out = run_spmf(program, input_file, algorithm, min_support, output_file)
        dt = round((time.time() - s)*1000) # to ms
        res = process_spmf_stdout(out, print_out)
        res['real_time'] = dt
        return res
    if program.endswith('miner'):
        if isinstance(min_support, float):
            min_support = sequence_miner_min_support(min_support, input_file)
        s = time.time()
        out = run_sequence_miner(program, input_file, algorithm, min_support, output_file)
        dt = round((time.time() - s)*1000) # to ms
        res = process_sequence_miner_stdout(out, print_out)
        res['real_time'] = dt
        return res
    return None


def compare_output_files(seq_miner_out, spmf_out):
    spmf_lines = []
    with open(spmf_out, 'r') as f:
        for _, l in enumerate(f):
            spmf_lines.append(l.split('-1')[0].strip())
    spmf_lines.sort()

    sm_lines = []
    with open(seq_miner_out, 'r') as f:
        for _, l in enumerate(f):
            sm_lines.append(l.strip())

    return set(spmf_lines) == set(sm_lines)