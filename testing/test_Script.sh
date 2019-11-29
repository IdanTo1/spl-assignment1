make debug
#./bin/splflix testing/config1.json < testing/check1.in > testing/results/check1.log
valgrind --log-file="testing/results/valgrind2.log" ./bin/splflix testing/testLen1.json < testing/check2.in > testing/results/check2.log
valgrind --log-file="testing/results/valgrind3.log" ./bin/splflix testing/testLen1.json < testing/check3.in > testing/results/check3.log
valgrind --log-file="testing/results/valgrind4.log" ./bin/splflix testing/testLen2.json < testing/check4.in > testing/results/check4.log
valgrind --log-file="testing/results/valgrind5.log" ./bin/splflix testing/testRer1.json < testing/check5.in > testing/results/check5.log
valgrind --log-file="testing/results/valgrind6.log" ./bin/splflix testing/testRer2.json < testing/check6.in > testing/results/check6.log
valgrind --log-file="testing/results/valgrind7.log" ./bin/splflix testing/testGen.json < testing/check7.in > testing/results/check7.log
valgrind --log-file="testing/results/valgrind8.log" ./bin/splflix testing/testEpisode1.json < testing/check8.in > testing/results/check8.log
valgrind --log-file="testing/results/valgrind9.log" ./bin/splflix testing/testEpisode2.json < testing/check9.in > testing/results/check9.log
valgrind --log-file="testing/results/valgrind10.log" ./bin/splflix testing/config1.json < testing/check10.in > testing/results/check10.log
echo diffing files
#diff testing/check1.out testing/results/check1.log
diff testing/check2.out testing/results/check2.log
diff testing/check3.out testing/results/check3.log
diff testing/check4.out testing/results/check4.log
#diff testing/check5.out testing/results/check5.log
#diff testing/check6.out testing/results/check6.log
#diff testing/check7.out testing/results/check7.log
diff testing/check8.out testing/results/check8.log
diff testing/check9.out testing/results/check9.log
diff testing/check10.out testing/results/check10.log
echo finished