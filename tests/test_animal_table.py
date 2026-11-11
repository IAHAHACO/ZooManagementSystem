import argparse
import json
import os
from pathlib import Path
import subprocess
import shutil

HERE = Path(__file__).resolve().parent
HEADERS = ['ID', 'Кличка', 'Вид', 'Стать', 'Вік', 'Вольєр', 'Останнє годування']

def check_output(result):
    checks = []
    def check(name, value, detail=''):
        checks.append({'check': name, 'passed': bool(value), 'detail': detail})
    out = result['output']
    expected = result['expected_fields']
    check('all_source_fields_unchanged', result['unchanged'])
    if result['kind'] == 'search_empty':
        check('empty_search_message', 'У списку немає даних для пошуку!' in out)
        check('no_table_for_empty_search', '|' not in out)
        return checks
    if result['kind'] == 'search_absent':
        check('absent_id_message', 'Тварину з ID 10000003 не вдалось знайти.' in out)
        check('no_unrelated_animals', 'Лео' not in out and 'Мія' not in out)
        return checks
    if not expected:
        check('empty_list_message', 'Вивід пустого списку неможливий!' in out)
        check('no_table_for_empty_list', '|' not in out)
        return checks
    for row in expected:
        for i, field in enumerate(row):
            check('field_preserved_' + row[0] + '_' + str(i), field in out, field)
        # Format independent reproduction: no boundary at all between two long strings.
        check('adjacent_name_species_not_merged_' + row[0], row[1] + row[2] not in out)
    cleaned = out.replace('Введіть ID тварини: ', '')
    lines = [line.rstrip('\r') for line in cleaned.splitlines() if '|' in line]
    # Accept either outer delimiters or delimiters only between columns.
    def cells(line):
        parts = line.split('|')
        if parts and not parts[0].strip(): parts.pop(0)
        if parts and not parts[-1].strip(): parts.pop()
        return [p.strip() for p in parts]
    data_lines = [line for line in lines if cells(line) and cells(line)[0] in {r[0] for r in expected}]
    check('exact_record_count', len(data_lines) == len(expected), str(len(data_lines)))
    check('full_values_in_separate_columns', [cells(line) for line in data_lines] == expected)
    if result['kind'] == 'list':
        header_lines = [line for line in lines if cells(line) == HEADERS]
        check('all_seven_headers', len(header_lines) == 1)
        all_lines = header_lines + data_lines
        # Python indexes Unicode code points, so Cyrillic multibyte text cannot mask a width error.
        positions = [[i for i,ch in enumerate(line) if ch == '|'] for line in all_lines]
        check('unicode_column_separator_alignment', len(all_lines) == len(expected)+1 and
              bool(positions) and len(positions[0]) >= 6 and all(p == positions[0] for p in positions), str(positions))
    return checks

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--source', default=str(HERE.parent))
    parser.add_argument('--label', default='animal_table')
    parser.add_argument('--compiler', default=os.environ.get('CXX', 'g++'))
    parser.add_argument('--output-dir')
    args = parser.parse_args()
    source = Path(args.source).resolve()
    label = args.label
    compiler_path = shutil.which(args.compiler)
    if not compiler_path:
        raise SystemExit('C++ compiler not found: ' + args.compiler)
    compiler = Path(compiler_path).resolve()
    output = Path(args.output_dir).resolve() if args.output_dir else source / 'build' / 'tests'
    output.mkdir(parents=True, exist_ok=True)
    exe = output / (label + ('.exe' if os.name == 'nt' else ''))
    cmd = [str(compiler), '-std=c++17', '-Wall', '-Wextra', '-finput-charset=UTF-8', '-fexec-charset=UTF-8',
           '-I', str(source), str(HERE/'animal_table_harness.cpp'),
           str(source/'AnimalManager.cpp'), str(source/'CorrectInputChecker.cpp'), '-o', str(exe)]
    build = subprocess.run(cmd, capture_output=True, text=True, encoding='utf-8', errors='replace', timeout=120)
    (output/(label+'_build.log')).write_text(subprocess.list2cmdline(cmd)+'\n'+build.stdout+build.stderr, encoding='utf-8')
    if build.returncode:
        print(build.stderr)
        raise SystemExit(build.returncode)
    env = os.environ.copy()
    env['PATH'] = str(compiler.parent) + os.pathsep + env.get('PATH','')
    run = subprocess.run([str(exe)], capture_output=True, encoding='utf-8', env=env, timeout=10)
    (output/(label+'_stdout.json')).write_text(run.stdout, encoding='utf-8')
    if run.returncode:
        raise RuntimeError(f'harness exit {run.returncode}: {run.stderr}')
    results = json.loads(run.stdout)
    total = passed = 0
    for result in results:
        result['checks'] = check_output(result)
        result['status'] = 'PASS' if all(c['passed'] for c in result['checks']) else 'FAIL'
        total += len(result['checks'])
        passed += sum(c['passed'] for c in result['checks'])
    report = {'label':label, 'scenarios':len(results), 'passed_scenarios':sum(r['status']=='PASS' for r in results),
              'checks':total,'passed_checks':passed, 'results':results}
    (output/(label+'_results.json')).write_text(json.dumps(report,ensure_ascii=False,indent=2),encoding='utf-8')
    log=[]
    for r in results:
        log.append(f"=== {r['id']} {r['status']} ===\n{r['output']}\n")
        for c in r['checks']:
            if not c['passed']: log.append('FAIL '+c['check']+' '+c['detail']+'\n')
    (output/(label+'_readable.log')).write_text(''.join(log),encoding='utf-8')
    print(json.dumps({k:v for k,v in report.items() if k!='results'},ensure_ascii=False))
    for r in results:
        print(r['id'],r['status'],[c['check'] for c in r['checks'] if not c['passed']])
    if report['passed_scenarios'] != report['scenarios']:
        raise SystemExit(1)

if __name__ == '__main__': main()
