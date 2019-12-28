cd ..
export BASE=$(pwd)
cd bitcount

$BASE/scripts/gem5_scripts/run_cfi_nofwd.sh "./bitcnts" "75000"  "" 
$BASE/scripts/gem5_scripts/run_counter_nofwd.sh "./bitcnts" "75000" ""
$BASE/scripts/gem5_scripts/run_rowhammer_nofwd.sh "./bitcnts" "75000" ""
$BASE/scripts/gem5_scripts/run_shadow_nofwd.sh "./bitcnts" "75000" ""
$BASE/scripts/gem5_scripts/run_sanitizer_nofwd.sh "./bitcnts_a32" "75000" ""
cd $BASE/scripts
