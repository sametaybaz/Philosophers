/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaybaz <aaybaz@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 13:43:34 by aaybaz            #+#    #+#             */
/*   Updated: 2023/06/16 14:26:22 by aaybaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdlib.h>
# include <stdio.h>
# include <sys/time.h> 
# include <unistd.h>
# include <pthread.h>
# include <string.h>

typedef struct s_philo
{
	int					id_philo;
	int					num_eat_philo;
	int					fork_left;
	int					fork_right;
	long long			last_time_eat;
	struct s_game_rules	*rules;
	pthread_t			thread_id;
}t_philo;

typedef struct s_game_rules
{
	int				num_philos;
	int				time_death;
	int				time_eat;
	int				time_sleep;
	int				num_limit_eat;
	int				died;
	int				all_philos_ate;
	long long		start_time;
	pthread_mutex_t	check_eat;
	pthread_mutex_t	*forks;
	pthread_mutex_t	write_status;
	t_philo			*philosophers;
}t_game_rules;

int			ft_atoi(const char *str);
long long	timestamp(void);
long long	time_result(long long start, long long finish);
void		sleep_philo(long long time, t_game_rules *rules);
void		print_status(t_game_rules *rules, int id, char *str);
int			start_dinner(t_game_rules *rules);
void		eat_philo(long long time, t_game_rules *rules);

#endif
